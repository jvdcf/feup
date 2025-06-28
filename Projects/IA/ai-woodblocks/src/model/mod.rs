//! The model representation of the game.
//!
//! Here contains all the data structures and logic for the WoodBlocks game.
//! A moment of a game is represented by a `State` struct, which contains the current state of the `Board`, `Batch` of pieces, the `List` of upcoming batches, the current `Score` and `combo` of the player.
//! This model represents the following rules and game logic:
//! - The player has batches of 3 pieces to place in the board in any order;
//! - All batches are visible to the player;
//! - To be able to use new pieces, it is necessary to place all 3 pieces first
//! - It is forbidden to rotate the pieces;
//! - When a row or column of the board is full, that line is cleared;
//! - The game is lost when is impossible to place one piece.
//! - The objective is to place all the pieces from the list with the best score possible.

use pos::Pos;
use serde::{Deserialize, Serialize};
use thiserror::Error;

pub mod batch;
pub mod board;
pub mod piece;
pub mod pos;
pub mod state;

/// Describes a movement in game.
#[derive(Debug, Serialize, Deserialize, Clone, Copy)]
pub struct Movement {
    pub piece_idx: usize,
    pub pos: Pos,
    pub cost: f64,
}

impl Default for Movement {
    fn default() -> Self {
        Self {
            piece_idx: usize::MAX,
            pos: Pos::default(),
            cost: f64::MAX,
        }
    }
}

/// Describes an error in game.
#[derive(Debug, Error)]
pub enum GameError {
    #[error("Unable to put block at position ({x}, {y}) of the Board", x=.0.x, y=.0.y)]
    Placement(Pos),
    #[error("Board index out of bounds")]
    Bound,
    #[error("Piece not found in Batch")]
    PieceNotFound,
}
