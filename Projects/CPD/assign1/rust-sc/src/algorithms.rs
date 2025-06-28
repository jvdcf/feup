use crate::matrix::*;

pub fn seq_simple_mul<const M: usize, const N: usize, const P: usize>(
    lhs: &Matrix<M, N>,
    rhs: &Matrix<N, P>,
) -> Matrix<M, P>
where
    [(); M * N]:,
    [(); N * P]:,
    [(); M * P]:,
{
    let mut res: Matrix<M, P> = Matrix::default(); // Default sets to zeros.
    for i in 0..M {
        for j in 0..P {
            let mut accum = 0.;
            for k in 0..N {
                accum += lhs[(i, k)] * rhs[(k, j)];
            }
            res[(i, j)] = accum;
        }
    }
    res
}

pub fn seq_line_mul<const M: usize, const N: usize, const P: usize>(
    lhs: &Matrix<M, N>,
    rhs: &Matrix<N, P>,
) -> Matrix<M, P>
where
    [(); M * N]:,
    [(); N * P]:,
    [(); M * P]:,
{
    let mut res: Matrix<M, P> = Matrix::default(); // Default sets to zeros.
    for j in 0..P {
        for k in 0..N {
            for i in 0..M {
                res[(i, j)] += lhs[(i, k)] * rhs[(k, j)];
            }
        }
    }
    res
}
