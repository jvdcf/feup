# Woodblocks: Heuristic Search Methods implemented for a One-Player Solitaire Game

> Faculty of Engineering at University of Porto  
> Bachelor's Degree in Informatics and Computing Engineering  
> Artificial Intelligence (L.EIC029) 2024/2025
>
> - Luís Paulo Gonçalves dos Reis (Regent of the course)
> - Telmo João Vales Ferreira Barros (Theoretical-Practical classes)

> **Class 09; Group 02**
>
> - Duarte Souto Assunção (up202208319@up.pt)
> - Guilherme Duarte Silva Matos (up202208755@up.pt)
> - João Vítor da Costa Ferreira (up202208393@up.pt)

## Prerequisites

- Rust programming language and Cargo package manager (recommended v1.85 or later);
  > It is recommended to install Rust using [rustup](https://www.rust-lang.org/tools/install):
  >
  > ```bash
  > curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
  > ```
- A Unix-like operating system, preferably Linux;
  > This project was tested on Ubuntu, NixOS and Gentoo, so support for these systems is guaranteed.  
  > It should also work on a Windows system without WSL, but it is not guaranteed.
- 5GB of free disk space.
  > Rust dependencies can take a lot of space; to clean them, run `cargo clean` in the project folder.

## Usage

> NOTE: The project has a lot of dependencies; compiling for the first time may take a while.

#### CLI structure: (execute `cargo run -- play --help` to learn more)

```bash
cargo run -r -- play (--ai <algorithm> | --human) <save_file> [-o <output_file>]
```

- `--ai <algorithm>`: The AI algorithm solves the game given in the save file, prints in the terminal the benchmark results and shows a GUI with each step of the solution.
  There are 5 algorithms available:
  - `Dfs`: Depth First Search
  - `Bfs`: Breadth First Search
  - `Greedy`: Greedy Search
  - `A*`: A\* Search
  - `WA*`: Weighted A\* Search
- `--human`: Opens the problem in the state given by the save file and opens a GUI to play the game.
- `<save_file>`: Path to the save file to load. There are 5 save files generated in the `assets/levels` folder.
- `-o <output_file>`: An optional path to an output file to save a problem state. To save and exit, just press ESC in the GUI.

#### Examples

- Run the [level1.ron](assets/levels/level1.ron) save file as a human player:

```bash
cargo run -r -- play --human assets/levels/level1.ron
```

- Run the [level5.ron](assets/levels/level5.ron) save file with the A\* algorithm and save the output to output.ron:

```bash
cargo run -r -- play --ai A* assets/levels/level5.ron -o output.ron
```

## Available Save Files

- [level1.ron](assets/levels/level1.ron):
  Starts with a mostly full board, 2 blocks in the batch and an empty list.
- [level2.ron](assets/levels/level2.ron):
  An empty board with 3 blocks in the batch and an empty list.
- [level3.ron](assets/levels/level3.ron):
  An empty board with 2 blocks in the batch and 1 batch with 2 other blocks on the list.
- [level4.ron](assets/levels/level4.ron):
  An empty board with 4 full batches.
- [level5.ron](assets/levels/level5.ron):
  An empty board with 100 full batches randomly generated.

#### Generate a new random state
To generate a new level with N random batches, run:
```bash
cargo run -r -- gen <N> <output_file>
```
Run `cargo run -r -- gen --help` for more information.

## Code documentation

All code is documented inside the source code.

To generate a more user-friendly documentation, generate the [rustdoc](https://doc.rust-lang.org/rustdoc/index.html):

```bash
cargo doc --open
```

---

[License](LICENSE) | [Third Party Credits](THIRDPARTY.md)
