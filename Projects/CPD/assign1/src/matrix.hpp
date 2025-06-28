#ifndef CPD_MATRIX_HPP
#define CPD_MATRIX_HPP

#include <cstddef>
#include <ostream>
#include <stdint.h>
#include <utility>
#include <vector>

enum class FillMethod {
    ONE,
    INDEX,
    ZERO,
    NONE,
};

class Matrix {
    private:
        const size_t width;
        const size_t height;
        std::vector<double> data;

        bool check_bounds(size_t row, size_t col) const;

    public:
        Matrix(size_t rows, size_t cols, FillMethod method);
        size_t getWidth() const;
        size_t getHeight() const;
        double get(size_t x, size_t y) const;
        void set(size_t x, size_t y, double value);
        void increment(size_t x, size_t y, double value);
        static bool multiplication_check(const Matrix &a, const Matrix &b);
        friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix);
        constexpr inline double& operator[](std::pair<size_t, size_t> idx) {
            return this->data[idx.second * this->width + idx.first];
        }
        constexpr inline const double& operator[](std::pair<size_t, size_t> idx) const {
            return this->data[idx.second * this->width + idx.first];
        }
};

#endif // CPD_MATRIX_HPP
