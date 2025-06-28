use std::io::{self, Stdin, Write};

use crate::{algorithms::*, matrix::test_square_matrix};

enum Algorithm {
    EXIT,
    SIMPLE,
    LINE
}

fn ask_algorithm() -> Algorithm {
    let stdin: Stdin = io::stdin();
    let mut input: String = String::new();
    
    loop {
        input.clear();
        println!("\nAvailable algorithms:");
        println!("0. Exit.");
        println!("1. Simple Multiplication.");
        println!("2. Line Multiplication.");
        print!("Choose the algorithm: ");
        io::stdout().flush().unwrap();
        let err = stdin.read_line(&mut input);
        if let Err(e) = err {println!("ERROR: {e}"); continue;};
        match input.trim() {
            "0" => return Algorithm::EXIT,
            "1" => return Algorithm::SIMPLE,
            "2" => return Algorithm::LINE,
            _  => println!("ERROR: Invalid option! Value read: {}", input.as_str())
        }
    }
}

fn ask_dimensions() -> usize {
    let stdin: Stdin = io::stdin();
    let mut input: String = String::new();

    loop {
        input.clear();
        println!("\nAvailable dimensions:");
        println!("0. None.");
        println!("1. 600x600.");
        println!("2. 1000x1000.");
        println!("3. 1400x1400.");
        println!("4. 1800x1800.");
        println!("5. 2200x2200.");
        println!("6. 2600x2600.");
        println!("7. 3000x3000.");
        print!("Choose the dimensions: ");
        io::stdout().flush().unwrap();
        let err = stdin.read_line(&mut input);
        if let Err(e) = err {println!("ERROR: {e}"); continue;};
        match input.trim() {
            "0" => return 0,
            "1" | "600" => return 600,
            "2" | "1000" => return 1000,
            "3" | "1400" => return 1400,
            "4" | "1800" => return 1800,
            "5" | "2200" => return 2200,
            "6" | "2600" => return 2600,
            "7" | "3000" => return 3000,
            _ => println!("ERROR: Invalid dimensions!")
        }
    }
}

pub fn run() {
    loop {
        match ask_algorithm() {
            Algorithm::EXIT => return,
            Algorithm::SIMPLE => {
                loop {
                    match ask_dimensions() {
                        600 => test_square_matrix::<600,_>(seq_simple_mul),
                        1000 => test_square_matrix::<1000,_>(seq_simple_mul),
                        1400 => test_square_matrix::<1400,_>(seq_simple_mul),
                        1800 => test_square_matrix::<1800,_>(seq_simple_mul),
                        2200 => test_square_matrix::<2200,_>(seq_simple_mul),
                        2600 => test_square_matrix::<2600,_>(seq_simple_mul),
                        3000 => test_square_matrix::<3000,_>(seq_simple_mul),
                        0 | _ => break,
                    }
                }
            },
            Algorithm::LINE => {
                loop {
                    match ask_dimensions() {
                        600 => test_square_matrix::<600,_>(seq_line_mul),
                        1000 => test_square_matrix::<1000,_>(seq_line_mul),
                        1400 => test_square_matrix::<1400,_>(seq_line_mul),
                        1800 => test_square_matrix::<1800,_>(seq_line_mul),
                        2200 => test_square_matrix::<2200,_>(seq_line_mul),
                        2600 => test_square_matrix::<2600,_>(seq_line_mul),
                        3000 => test_square_matrix::<3000,_>(seq_line_mul),
                        0 | _ => break,
                    }
                }
            },
        }
    }
}
