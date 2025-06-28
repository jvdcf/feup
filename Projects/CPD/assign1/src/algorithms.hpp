#ifndef CPD_ALGORITHMS_HPP
#define CPD_ALGORITHMS_HPP

#include "matrix.hpp"

namespace algorithms {
    /**
     * @brief Sequential simple matrix multiplication
     * @details The basic algorithm given by the CPD course, adapted to the Matrix class
     */
    void seq_simple_mult(const Matrix &a, const Matrix &b, Matrix &res);

    /**
     * @brief Sequential line matrix multiplication
     * @details "A version that multiplies an element from the first matrix by the correspondent line of the second matrix"
     */
    void seq_line_mult(const Matrix &a, const Matrix &b, Matrix &res);

    /**
     * @brief Sequential block matrix multiplication
     * @details "A block oriented algorithm that divides the matrices in blocks and uses the same sequence of computation as in seq_line_mult()"
     */
    void seq_block_mult(const Matrix &a, const Matrix &b, Matrix &res, size_t block_size);

    /**
     * @brief Parallel simple matrix multiplication
     * @details Parallel version of seq_simple_mult()   
     */
    void par_simple_mult(const Matrix &a, const Matrix &b, Matrix &res);

    /**
     * @brief Parallel line matrix multiplication
     * @details Parallel version of seq_line_mult()
     */
    void par_line_mult(const Matrix &a, const Matrix &b, Matrix &res);
};

#endif // CPD_ALGORITHMS_HPP
