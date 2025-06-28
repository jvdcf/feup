use std::fmt::Display;
use std::hash::Hash;

use serde::{Deserialize, Serialize};

use super::GameError;
use super::piece::Piece;
use super::pos::Pos;

pub const BOARD_SIDE: usize = 8;

#[derive(Copy, Clone, Debug, PartialEq, Eq, Hash, Deserialize, Serialize)]
#[repr(transparent)]
pub struct Board(u64); // [bool; BOARD_SIDE * BOARD_SIDE]

impl Default for Board {
    #[inline(always)]
    fn default() -> Self {
        Board(0)
    }
}

impl Board {
    /// Creates a new board from a list of booleans.
    pub fn new(vec: [bool; BOARD_SIDE * BOARD_SIDE]) -> Self {
        let mut board = Board(0);
        for (i, &val) in vec.iter().enumerate() {
            board
                .set((i % BOARD_SIDE, i / BOARD_SIDE).into(), val)
                .unwrap();
        }
        board
    }

    /// Returns whether the given position in the Board is occupied.
    pub fn get(&self, pos: Pos) -> Result<bool, GameError> {
        if pos.x >= BOARD_SIDE || pos.y >= BOARD_SIDE {
            return Err(GameError::Bound);
        }
        let mask = 1 << (pos.y * BOARD_SIDE + pos.x);
        let is_occupied = (self.0 & mask) != 0;

        Ok(is_occupied)
    }

    /// Sets the occupation of the given position in the Board as the given bool value.
    pub fn set(&mut self, pos: Pos, val: bool) -> Result<(), GameError> {
        if pos.x >= BOARD_SIDE || pos.y >= BOARD_SIDE {
            return Err(GameError::Bound);
        }
        let mask = 1 << (pos.y * BOARD_SIDE + pos.x);
        if val {
            self.0 |= mask;
        } else {
            self.0 &= !mask;
        }
        Ok(())
    }

    /// Check if all the positions are empty
    pub fn check(&self, pos: Pos, piece: &Piece) -> bool {
        for offset in piece {
            let cell_position = (pos.x + offset.x, pos.y + offset.y).into();
            match self.get(cell_position) {
                Ok(true) | Err(_) => return false,
                Ok(false) => {}
            }
        }
        true
    }

    /// Places a given piece in a given position. Does not remove the filled lines.
    pub fn place(&mut self, piece: &Piece, pos: Pos) -> Result<(), GameError> {
        if !self.check(pos, &piece) {
            return Err(GameError::Placement(pos));
        }

        for offset in piece {
            self.set((pos.x + offset.x, pos.y + offset.y).into(), true)?;
        }
        Ok(())
    }

    /// Cleans all the full rows and columns of the Board and returns how many were cleaned.
    pub fn clean(&mut self) -> u32 {
        // Collect full lines without clearing them
        let mut rows = Vec::new();
        for row in 0..BOARD_SIDE {
            if (0..BOARD_SIDE)
                .into_iter()
                .all(|col| self.get((col, row).into()).unwrap())
            {
                rows.push(row);
            }
        }
        let mut cols = Vec::new();
        for col in 0..BOARD_SIDE {
            if (0..BOARD_SIDE)
                .into_iter()
                .all(|row| self.get((col, row).into()).unwrap())
            {
                cols.push(col);
            }
        }
        let count = rows.len() + cols.len();

        // Clean lines
        for row in rows {
            for col in 0..BOARD_SIDE {
                self.set((col, row).into(), false).unwrap();
            }
        }
        for col in cols {
            for row in 0..BOARD_SIDE {
                self.set((col, row).into(), false).unwrap();
            }
        }
        count as u32
    }

    /// Returns a vector with the valid moves of the given piece
    pub fn valid_moves(&self, piece: &Piece) -> Vec<Pos> {
        let mut ret = Vec::<Pos>::new();
        for x in 0..BOARD_SIDE {
            for y in 0..BOARD_SIDE {
                let pos = (x, y).into();
                if self.check(pos, piece) {
                    ret.push(pos);
                }
            }
        }
        ret
    }
}

impl Display for Board {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        writeln!(f, "----------")?;
        for y in 0..BOARD_SIDE {
            write!(f, "|")?;
            for x in 0..BOARD_SIDE {
                write!(
                    f,
                    "{}",
                    if self.get((x, y).into()).expect("ERROR: Board Print") {
                        "■"
                    } else {
                        " "
                    }
                )?;
            }
            writeln!(f, "|")?;
        }
        writeln!(f, "----------")?;
        Ok(())
    }
}

mod tests {

    #[test]
    fn test_eq() {
        use super::Board;
        let l = Board::new([
            false, false, false, false, false, false, false, false, true, true, true, true, true,
            true, true, false, true, true, true, true, true, true, true, false, true, true, true,
            true, true, true, true, false, true, true, true, true, true, true, true, false, false,
            false, false, false, false, false, false, false, false, false, false, false, false,
            false, false, false, false, false, false, false, false, false, false, false,
        ]);
        let r = Board::new([
            false, false, false, false, false, false, false, false, true, true, true, true, true,
            true, true, false, true, true, true, true, true, true, true, false, true, true, true,
            true, false, true, true, false, // <-- Changed
            true, true, true, true, true, true, true, false, false, false, false, false, false,
            false, false, false, false, false, false, false, false, false, false, false, false,
            false, false, false, false, false, false, false,
        ]);
        assert_ne!(l, r);
    }
}
