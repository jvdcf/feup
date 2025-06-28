use super::{Algorithm, Answer};
use crate::algorithms::astar::{AStar, HeuristicNode};
use crate::model::state::State;
use petgraph::{Direction::Incoming, Graph, graph::NodeIndex};
use std::collections::BinaryHeap;

pub struct WeightedAStar;

impl Algorithm for WeightedAStar {
    /// WAstar search.
    fn search(start: State) -> Answer {
        let mut graph = Graph::new();
        let root = graph.add_node(start.clone());
        let mut path: Vec<NodeIndex> = vec![];
        let mut score = 0;
        let mut win_node = root;

        // Priority queue to store the heuristic and the node index of a state.
        let mut heap: BinaryHeap<HeuristicNode> = BinaryHeap::new();
        heap.push(HeuristicNode {
            heuristic: start.score as f64
                + AStar::board_dispersion(&start.board) / 2.0
                + AStar::line_completeness(&start.board) * 4.0,
            node: root,
        });

        // Examine the frontier with lower cost nodes first (min-heap)
        while let Some(hn) = heap.pop() {
            let state = graph[hn.node].clone();
            let valid_moves = state.valid_moves();
            if state.is_won() {
                win_node = hn.node;
                score = state.score;
                break;
            }

            for (piece_idx, pos) in valid_moves {
                let mut new_state = state.clone();
                new_state.play(piece_idx, pos).unwrap();
                let cost: f64 = 1. / (new_state.score - state.score) as f64;
                let new_hn = HeuristicNode {
                    heuristic: new_state.score as f64
                        + AStar::board_dispersion(&new_state.board) / 2.0
                        + AStar::line_completeness(&new_state.board) * 4.0,
                    node: graph.add_node(new_state),
                };
                // println!("score: {};\theuristic: {};", graph[new_hn.node].score, new_hn.heuristic);
                graph.add_edge(
                    hn.node,
                    new_hn.node,
                    super::Movement {
                        piece_idx,
                        pos,
                        cost,
                    },
                );
                heap.push(new_hn);
            }
        }

        let mut curr = win_node;
        path.push(curr);
        while let Some(pred) = graph.neighbors_directed(curr, Incoming).nth(0) {
            path.push(pred);
            curr = pred;
        }
        path.reverse();

        Answer { graph, path, score }
    }
}
