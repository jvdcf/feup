//! ## Woodblocks: Heuristic Search Methods implemented for a One-Player Solitaire Game
//!
//! Faculty of Engineering at University of Porto  
//! Bachelor's Degree in Informatics and Computing Engineering  
//! Artificial Intelligence (L.EIC029) 2024/2025  
//! - Luís Paulo Gonçalves dos Reis (Regent of the course)
//! - Telmo João Vales Ferreira Barros (Theoretical-Practical classes)
//!
//! **Class 09; Group 02**
//! - Duarte Souto Assunção (up202208319@up.pt)
//! - Guilherme Duarte Silva Matos (up202208755@up.pt)
//! - João Vítor da Costa Ferreira (up202208393@up.pt)

use algorithms::Benchmark;
use anyhow::anyhow;
use bevy::prelude::*;
use clap::{Args, Parser, Subcommand};
use model::Movement;
use model::batch::Batch;
use model::piece::Piece;
use model::pos::Pos;
use model::state::State;
use rand::Rng;
use serde::{Deserialize, Serialize};
use std::fs::File;
use std::io::{Read, Write};
use std::path::PathBuf;
use std::str::FromStr;

pub mod algorithms;
pub mod gui;
pub mod model;

/// Enum that selects which algorithm to run.
#[derive(Debug, Clone)]
pub enum Algorithm {
    Dfs,
    Bfs,
    AStar,
    WAStar,
    Greedy,
}

impl std::fmt::Display for Algorithm {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Algorithm::Dfs => write!(f, "Dfs"),
            Algorithm::Bfs => write!(f, "Bfs"),
            Algorithm::AStar => write!(f, "A*"),
            Algorithm::WAStar => write!(f, "Weighted A*"),
            Algorithm::Greedy => write!(f, "Greedy"),
        }
    }
}

/// Represents an error in parsing an algorithm.
#[derive(Clone, Debug, thiserror::Error)]
#[error("The value '{0}' is not a valid Algorithm. Try 'Dfs', 'Bfs', 'A*', 'WA*' or 'Greedy'.")]
pub struct AlgorithmParseError(String);

impl FromStr for Algorithm {
    type Err = AlgorithmParseError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        match s {
            "dfs" | "DFS" | "Dfs" => Ok(Self::Dfs),
            "bfs" | "BFS" | "Bfs" => Ok(Self::Bfs),
            "A*" | "a*" | "AStar" | "Astar" => Ok(Self::AStar),
            "Wa*" | "wa*" | "WA*" | "WAStar" | "WAstar" => Ok(Self::WAStar),
            "greedy" | "Greedy" | "GREEDY" => Ok(Self::Greedy),
            s => Err(AlgorithmParseError(s.to_string())),
        }
    }
}

/// Serializable struct that represents a save file.
#[derive(Debug, Clone, Deserialize, Serialize, Resource)]
pub struct SaveFile {
    pub init_state: State,
    pub bench: Option<Benchmark>,
    moves: Option<Vec<Movement>>,
}

impl SaveFile {
    pub fn insert_move(&mut self, m: Movement) {
        if let Some(v) = &mut self.moves {
            v.push(m);
        } else {
            self.moves = Some(vec![m]);
        }
    }
}

/// Commandline parsing struct that selects what kind of player is playing.
#[derive(Args)]
#[group(required = true)]
struct PlayerType {
    #[arg(short = 'A', long, group = "player")]
    /// Choose which AI to play this level.
    ai: Option<Algorithm>,
    #[arg(short = 'H', long, group = "player")]
    /// Assert a human is playing the game.
    human: bool,
}

#[derive(Subcommand)]
enum Select {
    /// Play the game.
    Play {
        #[command(flatten)]
        player: PlayerType,
        /// Input file specifying the level you wish to play.
        level: PathBuf,
        #[arg(short = 'o', long)]
        /// If you wish to save your progress, specify the path to a save file.
        save: Option<PathBuf>,
    },
    /// Generate a level.
    Gen {
        /// Number of batches the new level must have.
        batch_count: usize,
        /// The file to which output the generated level.
        output_file: PathBuf,
    },
}

#[derive(Parser)]
struct Cli {
    #[command(subcommand)]
    sel: Select,
}

/// Plays the game.
fn play_game(player: PlayerType, level: PathBuf, save: Option<PathBuf>) -> anyhow::Result<()> {
    let file_conts = {
        let mut state_file = std::fs::File::open(level)?;
        let mut file_conts = String::new();
        state_file.read_to_string(&mut file_conts)?;
        file_conts
    };
    let all_state: SaveFile = ron::from_str(&file_conts)?;
    let mut init_state = all_state.init_state.clone();
    if let Some(moves) = all_state.moves.clone() {
        for m in moves {
            let pos = m.pos;
            let piece_idx = m.piece_idx;
            init_state.play(piece_idx, pos)?;
        }
    }
    if player.human {
        App::new()
            .add_plugins(gui::HumanWoodBlocks {
                curr_state: init_state,
                file: all_state,
                output_path: save,
            })
            .run();
    } else if let Some(algo) = player.ai {
        App::new()
            .add_plugins(gui::AIWoodBlocks {
                file: all_state,
                curr_state: init_state,
                algo,
                output_path: save,
            })
            .run();
    } else {
        unreachable!();
    }
    Ok(())
}

/// Generates a level
fn generate_level(batch_count: usize, output_file: PathBuf) -> anyhow::Result<()> {
    use model::piece::GamePieces::*;
    let mut rng = rand::rng();
    let samples = [
        Q1, R2, I2, L3_000, L3_090, L3_180, L3_270, R3, I3, L4_000, L4_090, L4_180, L4_270, T4_000,
        T4_090, T4_180, T4_270, J4_000, J4_090, J4_180, J4_270, I4, R4, Q4, S4_000, S4_090, Z4_000,
        Z4_090,
    ];
    let mut list = Vec::new();
    for _ in 0..batch_count {
        list.push(Batch([
            Some(Piece::new(samples[rng.random_range(0..samples.len())])),
            Some(Piece::new(samples[rng.random_range(0..samples.len())])),
            Some(Piece::new(samples[rng.random_range(0..samples.len())])),
        ]));
    }

    let res = State {
        board: Default::default(),
        batch: list.pop().unwrap(),
        list,
        score: 0,
        combo: 1,
    };

    let save = SaveFile {
        init_state: res,
        bench: None,
        moves: None,
    };

    let mut f = File::create_new(output_file)?;
    f.write_all(ron::to_string(&save)?.as_bytes())?;

    Ok(())
}

fn main() -> anyhow::Result<()> {
    // Use things from here.
    let cli = Cli::parse();
    match cli.sel {
        Select::Play {
            player,
            level,
            save,
        } => play_game(player, level, save),
        Select::Gen {
            output_file,
            batch_count,
        } => {
            if batch_count == 0 {
                return Err(anyhow!("Cannot have 0 batches!"));
            }
            generate_level(batch_count.into(), output_file)
        }
    }
}
