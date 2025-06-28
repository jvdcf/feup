use std::collections::{HashMap, VecDeque};

use super::{Algorithm, Answer};
use crate::model::{Movement, state::State};
use petgraph::Graph;

pub struct BFS;

impl Algorithm for BFS {
    /// Performs BFS search
    fn search(start: State) -> Answer {
        let mut graph = Graph::new();
        let root = graph.add_node(start.clone());

        // BFS auxiliary variables
        let mut answers = Vec::new();
        let mut visited = Vec::new();
        let mut queue = VecDeque::new();
        queue.push_back(root);
        let mut state_map = HashMap::new();

        // BFS visits
        while let Some(node) = queue.pop_front() {
            if visited.contains(&node) {
                continue;
            }
            if graph[node].is_won() {
                answers.push(node);
            }
            if graph[node].is_game_over() {
                continue;
            }
            visited.push(node);

            let possible_moves = graph[node].valid_moves();

            // Add all possible moves to the graph as new nodes (if applicable) and to the queue
            for (piece_idx, pos) in possible_moves {
                let mut new_state = graph[node].clone();
                new_state.play(piece_idx, pos).unwrap();
                let cost: f64 = 1. / (new_state.score - graph[node].score) as f64;
                let edge = Movement {
                    piece_idx,
                    pos,
                    cost,
                };

                if let Some(next_node) = state_map.get(&new_state) {
                    graph.add_edge(node, *next_node, edge);
                    queue.push_back(*next_node);
                } else {
                    let new_node = graph.add_node(new_state);
                    graph.add_edge(node, new_node, edge);
                    queue.push_back(new_node);
                    state_map.insert(graph[new_node].clone(), new_node);
                }
            }
        }

        // Find the best path
        let best_node = answers
            .into_iter()
            .max_by(|a, b| graph[*a].score.cmp(&graph[*b].score))
            .expect("BFS did not find an answer.");
        let score = graph[best_node].score;

        // Build the path
        let mut path = Vec::new();
        path.push(best_node);
        let mut curr_node = best_node;
        while let Some(parent) = graph
            .neighbors_directed(curr_node, petgraph::Direction::Incoming)
            .next()
        {
            path.push(parent);
            curr_node = parent;
        }
        path.reverse();

        Answer { graph, path, score }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::model::batch::Batch;
    use crate::model::piece::{GamePieces, Piece};
    use crate::model::state::State;

    #[test]
    fn test_bfs_easy() {
        let start = State {
            board: Default::default(),
            batch: Batch([
                Some(Piece::new(GamePieces::R4)),
                Some(Piece::new(GamePieces::R2)),
                None,
            ]),
            list: vec![Batch([Some(Piece::new(GamePieces::L4_180)), None, None])],
            score: 0,
            combo: 1,
        };

        let answer = BFS::search(start);
        for node in answer.path.iter() {
            println!("{}", answer.graph[*node]);
        }
        assert!(answer.score == 20);
    }
}
