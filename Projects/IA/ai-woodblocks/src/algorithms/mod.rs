//! Uninformed and heuristic search algorithms for the WoodBlock puzzle.
//!
//! Here are defined various search algorithms with the goal of benchmarking them against each other and comparing uninformed and heuristic searches.
//! Each `Algorithm` is a struct that implements the `search()` method and has access to the `run()` that returns a `Benchmark` struct with the metrics inside.

use petgraph::{Graph, graph::NodeIndex, visit::IntoNodeReferences};
use serde::{Deserialize, Serialize};
use std::fmt::Display;

use crate::model::{Movement, state::State};

pub mod astar;
pub mod bfs;
pub mod dfs;
pub mod greedy;
pub mod weighted_astar;

/// Describes the information needed for an Answer
pub struct Answer {
    pub graph: Graph<State, Movement>,
    pub path: Vec<NodeIndex>,
    pub score: u32,
}

/// Describes a full benchmark of an algorithm.
#[derive(Debug, Clone, Deserialize, Serialize)]
pub struct Benchmark {
    pub score: u32,
    pub time: f64,
    pub states: usize,
    pub memory: usize,
}

impl Display for Benchmark {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        // Memory formatting
        let mut bytes = self.memory;
        let mut kbytes = bytes / 1000;
        bytes %= 1000;
        let mbytes = kbytes / 1000;
        kbytes %= 1000;
        let mut res = String::new();
        if mbytes > 0 {
            res.push_str(&format!("{}MB ", mbytes));
        }
        if kbytes > 0 {
            res.push_str(&format!("{}KB ", kbytes));
        }
        res.push_str(&format!("{}B", bytes));

        write!(
            f,
            "Score: {}, Time: {:.3}s, States: {}, Memory: {res}",
            self.score, self.time, self.states,
        )
    }
}

impl From<Answer> for Vec<Movement> {
    fn from(value: Answer) -> Self {
        let mut res = Vec::new();
        let mut prev = value.path[0];
        for idx in &value.path[1..] {
            let idx = *idx;
            res.push(
                value
                    .graph
                    .edges_connecting(prev, idx)
                    .next()
                    .expect("Literally cannot be empty bruh.")
                    .weight()
                    .clone(),
            );
            prev = idx;
        }
        res
    }
}

pub trait Algorithm {
    /// Runs an algorithm begining on a given state.
    fn run(start: State) -> (Benchmark, Vec<Movement>) {
        let start_time = std::time::Instant::now();

        let answer = Self::search(start);

        let time = start_time.elapsed().as_secs_f64();
        let score = answer.score;
        let states = answer.graph.node_count();
        let node_memory: usize = answer
            .graph
            .node_references()
            .into_iter()
            .map(|n| std::mem::size_of_val(n.1))
            .sum();
        let edge_memory: usize = answer
            .graph
            .edge_references()
            .into_iter()
            .map(|e| std::mem::size_of_val(e.weight()))
            .sum();
        let memory = node_memory + edge_memory;

        (
            Benchmark {
                score,
                time,
                states,
                memory,
            },
            answer.into(),
        )
    }

    fn search(start: State) -> Answer;
}

/// Note: This is not a test! Just a way to execute the benchmarks without the GUI.
/// For that, run `cargo test -r benchmark -- --nocapture`
mod tests {

    #[test]
    fn benchmark() {
        use crate::model::batch::Batch;
        use crate::model::board::Board;

        use crate::model::piece::{GamePieces, Piece};
        use crate::model::state::State;

        use crate::algorithms::*;

        fn state_level1() -> State {
            State {
                board: Board::new([
                    false, false, false, false, false, false, false, false, true, true, true, true,
                    true, true, true, false, true, true, true, true, true, true, true, false, true,
                    true, true, true, true, true, true, false, true, true, true, true, true, true,
                    true, false, false, false, false, false, false, false, false, false, false,
                    false, false, false, false, false, false, false, false, false, false, false,
                    false, false, false, false,
                ]),
                batch: Batch([
                    Some(Piece::new(GamePieces::I4)),
                    Some(Piece::new(GamePieces::R4)),
                    None,
                ]),
                list: vec![],
                score: 0,
                combo: 1,
            }
        }

        fn state_level2() -> State {
            State {
                board: Default::default(),
                batch: Batch([
                    Some(Piece::new(GamePieces::R4)),
                    Some(Piece::new(GamePieces::R2)),
                    None,
                ]),
                list: vec![Batch([Some(Piece::new(GamePieces::L4_180)), None, None])],
                score: 0,
                combo: 1,
            }
        }

        fn state_level3() -> State {
            State {
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
            }
        }

        #[allow(dead_code)]
        fn state_level4() -> State {
            State {
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
            }
        }

        let states = vec![state_level1(), state_level2(), state_level3()];

        for state in states {
            let greedy = greedy::Greedy::run(state.clone());
            println!("Greedy: {}", greedy.0);
            let dfs = dfs::DFS::run(state.clone());
            println!("DFS: {}", dfs.0);
            let bfs = bfs::BFS::run(state.clone());
            println!("BFS: {}", bfs.0);
            let astar = astar::AStar::run(state.clone());
            println!("AStar: {}", astar.0);
            let weighted_astar = weighted_astar::WeightedAStar::run(state.clone());
            println!("Weighted AStar: {}", weighted_astar.0);
            println!("---------------------------------");
        }
    }
}
