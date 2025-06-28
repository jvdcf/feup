#include "algorithms.hpp"
#include <cstddef>
#include <omp.h>

void algorithms::seq_simple_mult(const Matrix &a, const Matrix &b, Matrix &res) {
	const size_t a_width = a.getWidth();
	const size_t a_height = a.getHeight();
	const size_t b_height = b.getHeight();
	for (size_t i = 0; i < a_width; i++) {
		for (size_t j = 0; j < b_height; j++) {
			for (size_t k = 0; k < a_height; k++) {	 // a.getCols() == b.getRows()
				res[std::pair(i,j)] += a[std::pair(i,k)] * b[std::pair(k,j)];
			}
		}
	}
}

void algorithms::seq_line_mult(const Matrix &a, const Matrix &b, Matrix &res) {
	const size_t a_width = a.getWidth();
	const size_t a_height = a.getHeight();
	const size_t b_height = b.getHeight();	
	for (size_t j = 0; j < b_height; j++) {
		for (size_t k = 0; k < a_height; k++) {		// a.getCols() == b.getRows()
			for (size_t i = 0; i < a_width; i++) {
				res[std::pair(i,j)] += a[std::pair(i,k)] * b[std::pair(k,j)];
			}
		}
	}
}

void algorithms::seq_block_mult(const Matrix &a, const Matrix &b, Matrix &res, size_t block_size) {
	// seq_line_mult() adapted for block_size
	const size_t a_width = a.getWidth();
	const size_t a_height = a.getHeight();
	const size_t b_height = b.getHeight();	
	auto handle_block = [&a, &b, &res, block_size, b_height, a_height, a_width](size_t block_i, size_t block_j, size_t block_k) {
		for (size_t j = block_j; j < std::min(block_j + block_size, b_height); j++) {
			for (size_t k = block_k; k < std::min(block_k + block_size, a_height); k++) {	// a.getCols() == b.getRows()
				for (size_t i = block_i; i < std::min(block_i + block_size, a_width); i++) {
					res[std::pair(i,j)] += a[std::pair(i,k)] * b[std::pair(k,j)];
				}
			}
		}
	};

	for (size_t j = 0; j < b_height; j += block_size) {
		for (size_t k = 0; k < a_height; k += block_size) {	// a.getCols() == b.getRows()
			for (size_t i = 0; i < a_width; i += block_size) {
				handle_block(i, j, k);
			}
		}
	}
}

void algorithms::par_simple_mult(const Matrix &a, const Matrix &b, Matrix &res) {
	const size_t a_width = a.getWidth();
	const size_t a_height = a.getHeight();
	const size_t b_height = b.getHeight();	
	#pragma omp parallel for shared(a, b, res)
	for (size_t i = 0; i < a_width; i++) {
		#pragma omp parallel for shared(a, b, res)
		for (size_t j = 0; j < b_height; j++) {
			double temp = 0;
			for (size_t k = 0; k < a_height; k++) {	 // a.getCols() == b.getRows()
				temp += a[std::pair(i,k)] * b[std::pair(k,j)];
			}
			res[std::pair(i,j)] = temp;
		}
	}
}

void algorithms::par_line_mult(const Matrix &a, const Matrix &b, Matrix &res) {
	const size_t a_width = a.getWidth();
	const size_t a_height = a.getHeight();
	const size_t b_height = b.getHeight();	
	#pragma omp parallel for shared(a, b, res)
	for (size_t j = 0; j < b_height; j++) {
		#pragma omp parallel for shared(a, b, res)
		for (size_t k = 0; k < a_height; k++) {		// a.getCols() == b.getRows()
			for (size_t i = 0; i < a_width; i++) {
				res[std::pair(i,j)] += a[std::pair(i,k)] * b[std::pair(k,j)];
			}
		}
	}
}
