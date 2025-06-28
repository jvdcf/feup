# Assignment 1

## How to run the rust code
### Install Rust & Nightly
We recommend you to install via rustup since it is easier:  
```bash
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
```
If you want, there are other alternatives here: https://www.rust-lang.org/tools/install

Then, install nightly:
```bash
rustup toolchain install nightly
```
and set it as default for rust:
```bash
rustup default nightly
```

### Run Rust code with TUI
Got to the rust-sc directory and run in release:
```bash
cargo run -r
```

### Run Quickly Tests
Alternatively, you can run all the tests for an algorithm:
```bash
cargo run -r -- --all normal
```
```bash
cargo run -r -- --all line
```