use std::fmt::{Debug, Display};

use bevy::ecs::system::Resource;
use serde::{Deserialize, Serialize};

use super::batch::Batch;
use super::board::Board;
use super::pos::Pos;
use super::{GameError, Movement};

#[derive(Resource, Debug, Clone, Eq, PartialEq, Hash, Deserialize, Serialize)]
pub struct State {
    pub score: u32,
    pub combo: u32,
    pub batch: Batch,
    pub board: Board,
    pub list: Vec<Batch>,
}

impl State {
    pub fn play(&mut self, piece_idx: usize, pos: Pos) -> Result<Movement, GameError> {
        // Check legal move
        self.batch.has_piece(piece_idx)?;
        let p = self.batch.get(piece_idx).unwrap();
        if !self.board.check(pos, &p) {
            return Err(GameError::Placement(pos));
        }

        let init_score = self.score;

        // Apply move
        let p = self.batch.take(piece_idx);
        let block_size = p.block_size();
        self.board.place(&p, pos)?;
        let lines = self.board.clean();
        if lines > 0 {
            self.score += 5 * 2_u32.pow(lines) * self.combo;
        }
        self.score += block_size;

        self.combo = if lines > 0 { self.combo + 1 } else { 1 };

        if self.batch.is_empty() {
            self.batch = self.list.pop().unwrap_or_else(|| Batch([None, None, None]));
        }

        debug_assert!(
            self.score - init_score > 0,
            "Violated score principle. Any piece played must increase score"
        );
        let cost = 1. / (self.score - init_score) as f64;

        Ok(Movement {
            piece_idx,
            pos,
            cost,
        })
    }

    pub fn is_game_over(&self) -> bool {
        for piece in self.batch.0.iter() {
            if let Some(piece) = piece {
                if !self.board.valid_moves(piece).is_empty() {
                    return false;
                }
            }
        }
        true
    }

    #[inline]
    pub fn is_won(&self) -> bool {
        self.list.is_empty() && self.is_game_over()
    }

    /// Returns a vector with all the possible moves (piece to choose in batch: usize, position: Pos).
    pub fn valid_moves(&self) -> Vec<(usize, Pos)> {
        let mut moves = Vec::new();
        for i in 0..3 {
            if let Some(piece) = self.batch.get(i) {
                moves.extend(
                    self.board
                        .valid_moves(piece)
                        .into_iter()
                        .map(|pos| (i, pos)),
                );
            }
        }
        moves
    }
}

impl Display for State {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        writeln!(f, "")?;
        writeln!(f, "Score: {} (x{})", self.score, self.combo)?;
        writeln!(f, "{}", self.board)?;
        writeln!(f, "{}", self.batch)?;
        writeln!(f, "{} batches remaining.", self.list.len())?;
        writeln!(f, "")?;
        Ok(())
    }
}
