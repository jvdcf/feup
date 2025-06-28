use std::fmt::Display;

use bevy::ecs::component::Component;
use serde::{Deserialize, Serialize};

use super::pos::Pos;

#[derive(Debug, Clone, Eq, PartialEq, Hash, Serialize, Deserialize, Component)]
pub struct Piece(pub Vec<Pos>);

impl Piece {
    pub fn block_size(&self) -> u32 {
        self.0.len() as u32
    }

    pub fn new(p_type: GamePieces) -> Piece {
        match p_type {
            GamePieces::Q1 => Piece(vec![Pos { x: 0, y: 0 }]),
            GamePieces::R2 => Piece(vec![Pos { x: 0, y: 0 }, Pos { x: 1, y: 0 }]),
            GamePieces::I2 => Piece(vec![Pos { x: 0, y: 0 }, Pos { x: 0, y: 1 }]),
            GamePieces::L3_000 => Piece(vec![
                Pos { x: 0, y: 0 },
                Pos { x: 0, y: 1 },
                Pos { x: 1, y: 1 },
            ]),
            GamePieces::L3_090 => Piece(vec![
                Pos { x: 0, y: 1 },
                Pos { x: 1, y: 1 },
                Pos { x: 1, y: 0 },
            ]),
            GamePieces::L3_180 => Piece(vec![
                Pos { x: 0, y: 0 },
                Pos { x: 1, y: 0 },
                Pos { x: 1, y: 1 },
            ]),
            GamePieces::L3_270 => Piece(vec![
                Pos { x: 0, y: 0 },
                Pos { x: 0, y: 1 },
                Pos { x: 1, y: 0 },
            ]),
            GamePieces::R3 => Piece(vec![
                Pos { x: 0, y: 0 },
                Pos { x: 1, y: 0 },
                Pos { x: 2, y: 0 },
            ]),
            GamePieces::I3 => Piece(vec![
                Pos { x: 0, y: 0 },
                Pos { x: 0, y: 1 },
                Pos { x: 0, y: 2 },
            ]),
            GamePieces::L4_000 => Piece(vec![
                Pos { x: 0, y: 0 },
                Pos { x: 0, y: 1 },
                Pos { x: 0, y: 2 },
                Pos { x: 1, y: 2 },
            ]),
            GamePieces::L4_090 => Piece(vec![
                Pos { x: 2, y: 0 },
                Pos { x: 0, y: 1 },
                Pos { x: 1, y: 1 },
                Pos { x: 2, y: 1 },
            ]),
            GamePieces::L4_180 => Piece(vec![
                Pos { x: 0, y: 0 },
                Pos { x: 1, y: 0 },
                Pos { x: 1, y: 1 },
                Pos { x: 1, y: 2 },
            ]),
            GamePieces::L4_270 => Piece(vec![
                Pos { x: 0, y: 0 },
                Pos { x: 1, y: 0 },
                Pos { x: 2, y: 0 },
                Pos { x: 0, y: 1 },
            ]),
            GamePieces::T4_000 => Piece(vec![
                Pos { x: 1, y: 0 },
                Pos { x: 0, y: 1 },
                Pos { x: 1, y: 1 },
                Pos { x: 2, y: 1 },
            ]),
            GamePieces::T4_090 => Piece(vec![
                Pos { x: 1, y: 0 },
                Pos { x: 0, y: 1 },
                Pos { x: 1, y: 1 },
                Pos { x: 1, y: 2 },
            ]),
            GamePieces::T4_180 => Piece(vec![
                Pos { x: 0, y: 0 },
                Pos { x: 1, y: 0 },
                Pos { x: 2, y: 0 },
                Pos { x: 1, y: 1 },
            ]),
            GamePieces::T4_270 => Piece(vec![
                Pos { x: 0, y: 0 },
                Pos { x: 0, y: 1 },
                Pos { x: 1, y: 1 },
                Pos { x: 0, y: 2 },
            ]),
            GamePieces::J4_000 => Piece(vec![
                Pos { x: 1, y: 0 },
                Pos { x: 1, y: 1 },
                Pos { x: 0, y: 2 },
                Pos { x: 1, y: 2 },
            ]),
            GamePieces::J4_090 => Piece(vec![
                Pos { x: 0, y: 0 },
                Pos { x: 1, y: 0 },
                Pos { x: 2, y: 0 },
                Pos { x: 2, y: 1 },
            ]),
            GamePieces::J4_180 => Piece(vec![
                Pos { x: 0, y: 0 },
                Pos { x: 1, y: 0 },
                Pos { x: 0, y: 1 },
                Pos { x: 0, y: 2 },
            ]),
            GamePieces::J4_270 => Piece(vec![
                Pos { x: 0, y: 0 },
                Pos { x: 0, y: 1 },
                Pos { x: 1, y: 1 },
                Pos { x: 2, y: 1 },
            ]),
            GamePieces::I4 => Piece(vec![
                Pos { x: 0, y: 0 },
                Pos { x: 0, y: 1 },
                Pos { x: 0, y: 2 },
                Pos { x: 0, y: 3 },
            ]),
            GamePieces::R4 => Piece(vec![
                Pos { x: 0, y: 0 },
                Pos { x: 1, y: 0 },
                Pos { x: 2, y: 0 },
                Pos { x: 3, y: 0 },
            ]),
            GamePieces::Q4 => Piece(vec![
                Pos { x: 0, y: 0 },
                Pos { x: 1, y: 0 },
                Pos { x: 0, y: 1 },
                Pos { x: 1, y: 1 },
            ]),
            GamePieces::S4_000 => Piece(vec![
                Pos { x: 1, y: 0 },
                Pos { x: 2, y: 0 },
                Pos { x: 0, y: 1 },
                Pos { x: 1, y: 1 },
            ]),
            GamePieces::S4_090 => Piece(vec![
                Pos { x: 0, y: 0 },
                Pos { x: 0, y: 1 },
                Pos { x: 1, y: 1 },
                Pos { x: 1, y: 2 },
            ]),
            GamePieces::Z4_000 => Piece(vec![
                Pos { x: 0, y: 0 },
                Pos { x: 1, y: 0 },
                Pos { x: 1, y: 1 },
                Pos { x: 2, y: 1 },
            ]),
            GamePieces::Z4_090 => Piece(vec![
                Pos { x: 1, y: 0 },
                Pos { x: 0, y: 1 },
                Pos { x: 1, y: 1 },
                Pos { x: 0, y: 2 },
            ]),
        }
    }
}

impl IntoIterator for &Piece {
    type Item = Pos;
    type IntoIter = <Vec<Pos> as IntoIterator>::IntoIter;

    fn into_iter(self) -> Self::IntoIter {
        self.0.clone().into_iter()
    }
}

/// A collection of pieces available in the game.
/// Notation: <PieceType><Size>_<Rotation>
/// - Piece Types: (Follow the Tetris convention: https://en.wikipedia.org/wiki/Tetromino)
///    - Q: Square
///     ```
///     xx
///     xx
///     ```
///    - R: Row or I Piece rotated 90 degrees
///     ```
///     xxxx
///     ```
///    - I: Column or I Piece
///     ```
///     x
///     x
///     x
///     x
///     ```
///    - L: L Piece
///     ```
///     x
///     x
///     xx
///     ```
///    - T: T Piece
///     ```
///      x
///     xxx
///     ```
///    - J: J Piece
///     ```
///      x
///      x
///     xx
///     ```
///    - Z: Z Piece
///     ```
///     xx
///      xx
///     ```
///    - S: S Piece
///     ```
///      xx
///     xx
///     ```

#[derive(Debug, Clone, Copy)]
pub enum GamePieces {
    Q1,
    R2,
    I2,
    L3_000,
    L3_090,
    L3_180,
    L3_270,
    R3,
    I3,
    L4_000,
    L4_090,
    L4_180,
    L4_270,
    T4_000,
    T4_090,
    T4_180,
    T4_270,
    J4_000,
    J4_090,
    J4_180,
    J4_270,
    I4,
    R4,
    Q4,
    S4_000,
    S4_090,
    Z4_000,
    Z4_090,
}

impl Display for Piece {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        writeln!(f, "------")?;
        for y in 0..4 {
            write!(f, "|")?;
            for x in 0..4 {
                if self.0.contains(&Pos { x, y }) {
                    write!(f, "■")?;
                } else {
                    write!(f, " ")?;
                }
            }
            writeln!(f, "|")?;
        }
        Ok(())
    }
}
