#![feature(generic_const_exprs)]

use algorithms::{seq_line_mul, seq_simple_mul};
use matrix::test_square_matrix;

mod algorithms;
mod matrix;
mod perf;
mod tui;

fn main() {
    let args = std::env::args().collect::<Vec<_>>();
    match args
        .iter()
        .map(|x| x.as_str())
        .collect::<Vec<_>>()
        .as_slice()
    {
        [_] => tui::run(),
        [_, "--all", "normal"] | [_, "--all", "n"] => {
            test_square_matrix::<600, _>(seq_simple_mul);
            test_square_matrix::<1000, _>(seq_simple_mul);
            test_square_matrix::<1400, _>(seq_simple_mul);
            test_square_matrix::<1800, _>(seq_simple_mul);
            test_square_matrix::<2200, _>(seq_simple_mul);
            test_square_matrix::<2600, _>(seq_simple_mul);
            test_square_matrix::<3000, _>(seq_simple_mul);
        }
        [_, "--all", "line"] | [_, "--all", "l"] => {
            test_square_matrix::<600, _>(seq_line_mul);
            test_square_matrix::<1000, _>(seq_line_mul);
            test_square_matrix::<1400, _>(seq_line_mul);
            test_square_matrix::<1800, _>(seq_line_mul);
            test_square_matrix::<2200, _>(seq_line_mul);
            test_square_matrix::<2600, _>(seq_line_mul);
            test_square_matrix::<3000, _>(seq_line_mul);
        }
        _ => {
            eprintln!("Invalid arguments: {args:?}");
        }
    }
}
