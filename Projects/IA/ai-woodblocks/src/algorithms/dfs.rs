use super::{Algorithm, Answer};
use crate::model::{Movement, state::State};
use petgraph::{Graph, graph::NodeIndex};
use std::collections::HashMap;

pub struct DFS;

impl Algorithm for DFS {
    /// Performs DFS search.
    fn search(start: State) -> Answer {
        let mut graph = Graph::new();
        let root = graph.add_node(start.clone());
        let mut path = Vec::new();
        let mut state_map = HashMap::new();
        let score = DFS::dfs_visit(&mut graph, root, &mut path, &mut state_map);
        Answer { graph, path, score }
    }
}

impl DFS {
    /// Recursive dfs visit.
    fn dfs_visit(
        graph: &mut Graph<State, Movement>,
        node: NodeIndex,
        path: &mut Vec<NodeIndex>,
        map: &mut HashMap<State, NodeIndex>,
    ) -> u32 {
        let state = graph[node].clone();
        path.push(node);
        if state.is_won() {
            return state.score;
        }
        if state.is_game_over() {
            return 0;
        }

        let mut best_score = 0;
        let mut best_path = path.clone();
        let possible_moves = state.valid_moves();

        for (piece_idx, pos) in possible_moves {
            let mut new_state = state.clone();
            new_state.play(piece_idx, pos).unwrap();
            let cost: f64 = 1. / (new_state.score - state.score) as f64;
            let edge = Movement {
                piece_idx,
                pos,
                cost,
            };

            if let Some(next_node) = map.get(&new_state) {
                graph.add_edge(node, *next_node, edge);
                continue;
            }

            let new_node = graph.add_node(new_state);
            map.insert(graph[new_node].clone(), new_node);
            let mut remaining_visited = path.clone();
            graph.add_edge(node, new_node, edge);

            let score = DFS::dfs_visit(graph, new_node, &mut remaining_visited, map);
            if score > best_score {
                best_score = score;
                best_path = remaining_visited;
            }
        }

        path.clear();
        path.extend(best_path);
        return best_score;
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::model::batch::Batch;
    use crate::model::piece::{GamePieces, Piece};
    use crate::model::state::State;

    #[test]
    fn test_dfs_easy() {
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

        let answer = DFS::search(start);
        for node in answer.path.iter() {
            println!("{}", answer.graph[*node]);
        }
        assert!(answer.score == 20);
    }

    #[test]
    fn test_dfs_hard() {
        let start = State {
            board: Default::default(),
            batch: Batch([
                Some(Piece::new(GamePieces::I4)),
                Some(Piece::new(GamePieces::I4)),
                None,
            ]),
            list: vec![Batch([
                Some(Piece::new(GamePieces::Q4)),
                None,
                Some(Piece::new(GamePieces::Q4)),
            ])],
            score: 0,
            combo: 1,
        };

        let answer = DFS::search(start);
        for node in answer.path.iter() {
            println!("{}", answer.graph[*node]);
        }
        assert!(answer.score > 20);
    }
}
