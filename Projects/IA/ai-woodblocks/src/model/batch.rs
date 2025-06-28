use std::fmt::Display;

use super::GameError;
use super::piece::Piece;
use super::pos::Pos;
use bevy::ecs::system::Resource;
use serde::{Deserialize, Serialize};

#[derive(Resource, Debug, Clone, Eq, PartialEq, Hash, Deserialize, Serialize)]
pub struct Batch(pub [Option<Piece>; 3]);

impl Batch {
    pub fn is_empty(&self) -> bool {
        self.0.iter().all(|piece| piece.is_none())
    }

    pub fn has_piece(&self, index: usize) -> Result<(), GameError> {
        assert!(index < 3);
        if !self.0.get(index).unwrap().is_some() {
            return Err(GameError::PieceNotFound);
        }
        Ok(())
    }

    pub fn take(&mut self, index: usize) -> Piece {
        assert!(index < 3);
        self.0
            .get_mut(index)
            .unwrap()
            .take()
            .expect("No piece to take")
    }

    pub fn get(&self, index: usize) -> Option<&Piece> {
        assert!(index < 3);
        self.0.get(index).unwrap().as_ref()
    }
}

impl Display for Batch {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        writeln!(f, "------ ------ ------")?;
        for y in 0..4 {
            for piece in self.0.iter() {
                write!(f, "|")?;
                for x in 0..4 {
                    if let Some(piece) = piece {
                        if piece.0.contains(&Pos { x, y }) {
                            write!(f, "■")?;
                        } else {
                            write!(f, " ")?;
                        }
                    } else {
                        write!(f, " ")?;
                    }
                }
                write!(f, "| ")?;
            }
            writeln!(f, "")?;
        }
        writeln!(f, "------ ------ ------")?;
        Ok(())
    }
}
