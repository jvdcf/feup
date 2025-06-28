use super::{Algorithm, Answer};
use crate::model::{Movement, state::State};
use petgraph::{Graph, graph::NodeIndex};

pub struct Greedy;

impl Algorithm for Greedy {
    /// Greedy search
    fn search(start: State) -> Answer {
        let mut graph = Graph::new();
        let root = graph.add_node(start.clone());
        let mut path = vec![root];
        let score = Self::greedy_search(&mut graph, root, &mut path);
        Answer { graph, path, score }
    }
}

impl Greedy {
    /// Performs a greedy search choosing always the least expensive move.
    fn greedy_search(
        graph: &mut Graph<State, Movement>,
        node: NodeIndex,
        path: &mut Vec<NodeIndex>,
    ) -> u32 {
        let state = graph[node].clone();
        if state.is_won() | state.is_game_over() {
            return state.score;
        }

        let possible_moves = state.valid_moves();
        let mut best_state = state.clone();
        let mut best_move = Movement::default();
        for (piece_idx, pos) in possible_moves {
            let mut new_state = state.clone();
            let new_move = new_state.play(piece_idx, pos).unwrap();
            if new_move.cost < best_move.cost {
                best_state = new_state;
                best_move = new_move;
            }
        }

        let best_node = graph.add_node(best_state);
        graph.add_edge(node, best_node, best_move);
        path.push(best_node);
        return Self::greedy_search(graph, best_node, path);
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::model::batch::Batch;
    use crate::model::piece::{GamePieces, Piece};
    use crate::model::state::State;

    #[test]
    fn test_greedy_easy() {
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

        let answer = Greedy::search(start);
        for node in answer.path.iter() {
            println!("{}", answer.graph[*node]);
        }
        assert!(answer.score == 10);
    }

    #[test]
    fn test_greedy_hard() {
        let start = State {
            board: Default::default(),
            batch: Batch([
                Some(Piece::new(GamePieces::R4)),
                Some(Piece::new(GamePieces::R2)),
                Some(Piece::new(GamePieces::L4_180)),
            ]),
            list: vec![
                Batch([
                    Some(Piece::new(GamePieces::L4_180)),
                    Some(Piece::new(GamePieces::Q4)),
                    Some(Piece::new(GamePieces::R2)),
                ]),
                Batch([
                    Some(Piece::new(GamePieces::Q1)),
                    Some(Piece::new(GamePieces::S4_000)),
                    Some(Piece::new(GamePieces::Z4_090)),
                ]),
                Batch([
                    Some(Piece::new(GamePieces::L3_090)),
                    Some(Piece::new(GamePieces::J4_270)),
                    Some(Piece::new(GamePieces::T4_000)),
                ]),
            ],
            score: 0,
            combo: 1,
        };

        let answer = Greedy::search(start);
        for node in answer.path.iter() {
            println!("{}", answer.graph[*node]);
        }
    }
}
