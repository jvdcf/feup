# Diabetes Prediction: A Supervised Learning Project

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

- Rust programming language and Cargo package manager (recommended v1.85 or later stable version);
  > It is recommended to install Rust using [rustup](https://www.rust-lang.org/tools/install):
  >
  > ```bash
  > curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
  > ```
- Jupyter Notebook
  > Can be installed via `pip`, for example (see https://jupyter.org/install):
  >
  > ```bash
  > pip install notebook
  > ```
- EVCXR Jupyter Kernel for Rust
  > See https://github.com/evcxr/evcxr/blob/main/evcxr_jupyter/README.md for more
  > information about the installation process.
  >
  > ```bash
  > cargo install --locked evcxr_jupyter
  > evcxr_jupyter --install
  > rustup component add rust-src # Only needed if `rust-analyzer` is not installed
  > ```
- A Unix-like operating system, preferably Linux;
  > This project was tested on Ubuntu, NixOS and Gentoo, so support for these systems is guaranteed.  
  > It should also work on a Windows system without WSL, but it is not guaranteed.
- 2GB of free disk space.
  > Rust dependencies can take a lot of space; to clean them, run `cargo clean` in the project folder.

## Usage

### Notebook

Open [the notebook](./notebook.ipynb) in VSCode (With Jupyter extensions) or Jupyter Notebook,
select the "Rust" kernel and execute the cells in order.

Note that the first run may take several minutes to compile all the dependencies and 
auxiliar code. After that, successive runs will be a lot quicker until the kernel
is restarted or closed.

### CLI / Traditional

The same code inside the notebook can be found in [`main.rs`](./src/main.rs) for
a more traditional experience. To run, open a terminal and execute:
```bash
cargo run -r
```

---

[License](LICENSE) | [Third Party Credits](THIRDPARTY.md)
