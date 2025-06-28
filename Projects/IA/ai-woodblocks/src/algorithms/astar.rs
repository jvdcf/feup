use super::{Algorithm, Answer};
use crate::model::{
    board::{BOARD_SIDE, Board},
    pos::Pos,
    state::State,
};
use petgraph::{Direction::Incoming, Graph, graph::NodeIndex};
use std::collections::BinaryHeap;

/// ZST representing an ASTAR algorithm.
pub struct AStar;

/// Node carrying an heuristic.
#[derive(PartialOrd)]
pub struct HeuristicNode {
    pub heuristic: f64,
    pub node: NodeIndex,
}

impl Eq for HeuristicNode {}

impl Ord for HeuristicNode {
    fn cmp(&self, other: &Self) -> std::cmp::Ordering {
        if self.heuristic > other.heuristic {
            return std::cmp::Ordering::Less;
        }
        if self.heuristic < other.heuristic {
            return std::cmp::Ordering::Greater;
        }
        return self.node.cmp(&other.node);
    }
}

impl PartialEq for HeuristicNode {
    fn eq(&self, other: &Self) -> bool {
        self.node == other.node
    }
}

impl Algorithm for AStar {
    fn search(start: State) -> Answer {
        let mut graph = Graph::new();
        let root = graph.add_node(start.clone());
        let mut path: Vec<NodeIndex> = vec![];
        let mut score = 0;
        let mut win_node = root;
        // let mut wins = vec![];

        // Priority queue to store the heuristic and the node index of a state.
        let mut heap: BinaryHeap<HeuristicNode> = BinaryHeap::new();
        heap.push(HeuristicNode {
            heuristic: start.score as f64
                + AStar::board_dispersion(&start.board)
                + AStar::line_completeness(&start.board),
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
                // wins.push(hn.node);
                // continue;
            }

            for (piece_idx, pos) in valid_moves {
                // TODO: Check if this node was already explored and, in that case, try to relax edges.
                let mut new_state = state.clone();
                new_state.play(piece_idx, pos).unwrap();
                let cost: f64 = 1. / (new_state.score - state.score) as f64;
                let new_hn = HeuristicNode {
                    heuristic: new_state.score as f64
                        + AStar::board_dispersion(&new_state.board)
                        + AStar::line_completeness(&new_state.board),
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

        // if wins.len() == 0 {return Answer{graph, path: vec![], score:0};}

        let mut curr = win_node;
        // let mut curr = wins.iter().fold(wins[0], |max, n| {
        //     if graph[*n].score > graph[max].score {
        //         return *n;
        //     }
        //     max
        // }); // Get the winning state with higher score.
        // score = graph[curr].score;

        path.push(curr);
        while let Some(pred) = graph.neighbors_directed(curr, Incoming).nth(0) {
            path.push(pred);
            curr = pred;
        }
        path.reverse();

        Answer { graph, path, score }
    }
}

impl AStar {
    /// Recursive algorithm that for a given empty position in the board, calculates the empty area of that contains said position.
    fn erase_cluster(x: usize, y: usize, board: &mut Board) -> usize {
        board.set((x, y).into(), false).unwrap();
        let mut ret: usize = 1;
        if x != 0 && board.get((x - 1, y).into()).unwrap() {
            ret += Self::erase_cluster(x - 1, y, board);
        }
        if x < BOARD_SIDE - 1 && board.get((x + 1, y).into()).unwrap() {
            ret += Self::erase_cluster(x + 1, y, board);
        }
        if y != 0 && board.get((x, y - 1).into()).unwrap() {
            ret += Self::erase_cluster(x, y - 1, board);
        }
        if y < BOARD_SIDE - 1 && board.get((x, y + 1).into()).unwrap() {
            ret += Self::erase_cluster(x, y + 1, board);
        }
        return ret;
    }

    /// Returns the first empty position in the board
    fn search_next_full(board: &Board) -> Option<Pos> {
        for x in 0..BOARD_SIDE {
            for y in 0..BOARD_SIDE {
                if board.get((x, y).into()).unwrap() {
                    return Some(Pos { x, y });
                }
            }
        }
        None
    }

    /// Heuristic that returns how disperse is the board. A high number means low dispersion (i.e. high cost) and low number means high dispersion (i.e. low cost).
    pub fn board_dispersion(board: &Board) -> f64 {
        let mut board2 = board.clone();
        // let mut dispersion: f64 = 1.0;
        // let mut area: usize = 0;
        // let mut n: usize = 0;
        let mut clusters = 0;
        while let Some(Pos { x, y }) = Self::search_next_full(&mut board2) {
            // dispersion += 2usize.pow(Self::erase_cluster(x, y, &mut board2) as u32) as f64;
            // area += Self::erase_cluster(x, y, &mut board2);
            // n += 1;
            Self::erase_cluster(x, y, &mut board2);
            clusters += 1;
        }
        // assert!(dispersion != 0.0);
        // dispersion
        // f64::sqrt(area as f64) / n as f64
        (32 - clusters) as f64
    }

    /// Heuristic describing how close a line is to being completed.
    pub fn line_completeness(board: &Board) -> f64 {
        let mut blocks = 0;
        let mut lines = 0;
        for x in 0..BOARD_SIDE {
            for y in 0..BOARD_SIDE {
                let row = (0..BOARD_SIDE)
                    .map(|j| board.get((j, y).into()).unwrap())
                    .filter(|b| *b)
                    .count();
                let col = (0..BOARD_SIDE)
                    .map(|j| board.get((x, j).into()).unwrap())
                    .filter(|b| *b)
                    .count();
                let max = usize::max(row, col);
                if max > 0 {
                    blocks += max;
                    lines += 1;
                }
            }
        }
        blocks as f64 / lines as f64
    }
}
