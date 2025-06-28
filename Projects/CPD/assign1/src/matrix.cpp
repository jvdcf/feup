#include "matrix.hpp"
#include <iostream>
#include <algorithm>

Matrix::Matrix(size_t width, size_t height, FillMethod method): width(width), height(height) {
    size_t size = height * width;
    switch (method) {
        case FillMethod::ONE:
            this->data = std::vector<double>(size, 1);
            break;
        case FillMethod::INDEX:
            this->data = std::vector<double>(size);
            for (size_t row = 0; row < width; row++) {
                for (size_t col = 0; col < height; col++) {
                    this->set(row, col, row + 1);
                }
            }
            break;
        case FillMethod::NONE:
            this->data = std::vector<double>(size);
            break;
        case FillMethod::ZERO:
            this->data = std::vector<double>(size, 0);
            break;
    }
}

size_t Matrix::getWidth() const {
    return this->width;
}

size_t Matrix::getHeight() const {
    return this->height;
}

double Matrix::get(size_t x, size_t y) const {
    if (!this->check_bounds(x, y)) {
        throw std::out_of_range("Matrix::get() - Index out of bounds");
    }
    return this->data[y * this->getWidth() + x];
}

void Matrix::set(size_t x, size_t y, double value) {
    if (!this->check_bounds(x, y)) {
        throw std::out_of_range("Matrix::set() - Index out of bounds");
    }
    this->data[y * this->getWidth() + x] = value;
}

void Matrix::increment(size_t x, size_t y, double value) {
    if (!this->check_bounds(x, y)) {
        throw std::out_of_range("Matrix::increment() - Index out of bounds");
    }
    this->data[y * this->getWidth() + x] += value;
}

bool Matrix::check_bounds(size_t x, size_t y) const {
    return x < this->getWidth() && y < this->getHeight();
}

bool Matrix::multiplication_check(const Matrix &a, const Matrix &b) {
    return a.getHeight() == b.getWidth();
}

std::ostream &operator<<(std::ostream &os, const Matrix &matrix)
{
    os << "Result matrix: " << std::endl;
    for (size_t col = 0; col < std::min((size_t)10, matrix.getWidth()); col++) {
        os << matrix.get(0, col) << " ";
    }
    os << std::endl;
    return os;
}
