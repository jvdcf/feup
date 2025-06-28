use serde::{Deserialize, Serialize};

/// The position of a cell in a board.
#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash, Deserialize, Serialize, Default)]
pub struct Pos {
    /// The column index of the cell.
    pub x: usize,
    /// The row index of the cell.
    pub y: usize,
}

impl From<Pos> for (usize, usize) {
    fn from(value: Pos) -> Self {
        (value.x, value.y)
    }
}

impl From<(usize, usize)> for Pos {
    fn from(value: (usize, usize)) -> Self {
        Pos {
            x: value.0,
            y: value.1,
        }
    }
}
