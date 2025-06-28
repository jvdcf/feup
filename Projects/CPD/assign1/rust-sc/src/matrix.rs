use std::{
    ops::{Index, IndexMut, Mul},
    time::Instant,
};

use crate::perf;

// use rand::prelude::*;

#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct Matrix<const M: usize, const N: usize>
where
    [(); M * N]:,
{
    pub(crate) arr: Box<[f64; M * N]>,
}

impl<const M: usize, const N: usize> Matrix<M, N>
where
    [(); M * N]:,
{
    pub fn new_fn<F>(f: F) -> Self
    where
        F: Fn(usize) -> f64,
        [(); M * N]:,
    {
        let mut m = Self::default();
        for i in 0..M {
            for j in 0..N {
                m[(i, j)] = f(i + 1);
            }
        }
        m
    }
}

impl<const M: usize, const N: usize> Default for Matrix<M, N>
where
    [(); M * N]:,
{
    fn default() -> Self {
        let arr = vec![0.0; M * N].into_boxed_slice();
        let arr: Box<[f64; M * N]> = arr.try_into().expect("Infallible");
        Self { arr }
    }
}

impl<const M: usize, const N: usize> Index<(usize, usize)> for Matrix<M, N>
where
    [(); M * N]:,
{
    type Output = f64;

    fn index(&self, index: (usize, usize)) -> &Self::Output {
        let (x, y) = index;
        &self.arr[y * M + x]
    }
}

impl<const M: usize, const N: usize> IndexMut<(usize, usize)> for Matrix<M, N>
where
    [(); M * N]:,
{
    fn index_mut(&mut self, index: (usize, usize)) -> &mut Self::Output {
        let (x, y) = index;
        &mut self.arr[y * M + x]
    }
}

impl<const M: usize, const N: usize, const P: usize> Mul<Matrix<N, P>> for Matrix<M, N>
where
    [(); M * N]:,
    [(); M * P]:,
    [(); N * P]:,
{
    type Output = Matrix<M, P>;

    fn mul(self, rhs: Matrix<N, P>) -> Self::Output {
        let mut res: Self::Output = Matrix::default();
        for i in 0..M {
            for k in 0..P {
                let mut temp = 0.0;
                for j in 0..N {
                    temp += self[(i, j)] * rhs[(j, k)];
                }
                res[(i, k)] = temp;
            }
        }
        res
    }
}

impl<const M: usize, const N: usize, const P: usize> Mul<&Matrix<N, P>> for &Matrix<M, N>
where
    [(); M * N]:,
    [(); M * P]:,
    [(); N * P]:,
{
    type Output = Matrix<M, P>;

    fn mul(self, rhs: &Matrix<N, P>) -> Self::Output {
        let mut res: Self::Output = Matrix::default();
        for i in 0..M {
            for k in 0..P {
                let mut temp = 0.0;
                for j in 0..N {
                    temp += self[(i, j)] * rhs[(j, k)];
                }
                res[(i, k)] = temp;
            }
        }
        res
    }
}

impl<const M: usize, const N: usize> From<[[f64; M]; N]> for Matrix<M, N>
where
    [(); M * N]:,
{
    fn from(value: [[f64; M]; N]) -> Self {
        let mut res = Matrix::<M, N>::default();
        for i in 0..M {
            for j in 0..N {
                res[(i, j)] = value[j][i];
            }
        }
        res
    }
}

#[cfg(test)]
mod tests {
    use crate::matrix::Matrix;

    #[test]
    fn simple_mul() {
        let a = Matrix::from([[2.0, 3.0]]);
        let b = Matrix::from([[1.0], [2.0]]);
        let c = &b * &a;
        let d = a * b;

        assert_eq!(c, Matrix::from([[8.0]]));
        assert_eq!(d, Matrix::from([[2.0, 3.0], [4.0, 6.0]]));
    }
}

/* #[inline(always)]
fn matrix_randomize<const M: usize, const N: usize>(m: &mut Matrix<M, N>)
where
    [(); M * N]:,
{
    let mut rng = rand::rng();
    for i in 0..M {
        for j in 0..N {
            m[(i, j)] = rng.random::<f64>();
        }
    }
} */

#[inline(always)]
pub fn test_square_matrix<const M: usize, F: Fn(&Matrix<M, M>, &Matrix<M, M>) -> Matrix<M, M>>(f: F)
where
    [(); M * M]:,
{
    println!("{M}x{M} TEST:");

    print!("Initializing Matrices: ");

    let now = Instant::now();
    let a: Matrix<M, M> = Matrix::new_fn(|_| 1.);
    let b: Matrix<M, M> = Matrix::new_fn(|x| x as f64);
    let mut res = Default::default();
    let took = now.elapsed();
    println!("Took {}ms", took.as_millis());

    print!("Multiplying them:  ");
    match perf::perf_bench(|| {
        res = f(&a, &b);
    }) {
        Ok(_) => {}
        Err(e) => eprintln!("[ERROR]: {e}"),
    }
    print!("First 10 elements: [");
    for i in 0..9 {
        print!("{}, ", res[(0, i)]);
    }
    println!("{}]", res[(0, 9)]);
}
