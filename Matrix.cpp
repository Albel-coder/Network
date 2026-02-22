#include "Matrix.h"
#include <random>
#include <stdexcept>

Matrix::Matrix(size_type rows, size_type cols, value_type init)
    : data(rows* cols, init), rows_(rows), cols_(cols) {
    if (rows == 0 || cols == 0) {
        data.clear();
        rows_ = cols_ = 0;
    }
}

Matrix::value_type& Matrix::operator()(size_type i, size_type j)
{
    if (i >= rows_ || j >= cols_) 
        throw std::out_of_range("Matrix index out of range");

    return data[i * cols_ + j];
}

const Matrix::value_type& Matrix::operator()(size_type i, size_type j) const {
    return data[i * cols_ + j];
}

void Matrix::random() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<value_type> dis(0.0, 1.0);

    for (auto& val : data) {
        val = dis(gen);
    }
}

std::vector<Matrix::value_type> Matrix::multiply(const std::vector<value_type>& vec) const {
    if (cols_ != vec.size()) {
        throw std::runtime_error("Matrix multiply: dimension mismatch");
    }
    std::vector<value_type> result(rows_, value_type{});

    for (size_type i = 0; i < rows_; ++i) {
        value_type sum = 0;

        for (size_type j = 0; j < cols_; ++j) {
            sum += data[i * cols_ + j] * vec[j];
        }
        result[i] = sum;
    }

    return result;
}

std::vector<Matrix::value_type> Matrix::multiply_transposed(const std::vector<value_type>& vec) const {
    if (rows_ != vec.size()) {
        throw std::runtime_error("Matrix multiply_transposed: dimension mismatch");
    }
    std::vector<value_type> result(cols_, value_type{});

    for (size_type j = 0; j < cols_; ++j) {
        value_type sum = 0;

        for (size_type i = 0; i < rows_; ++i) {
            sum += data[i * cols_ + j] * vec[i];
        }
        result[j] = sum;
    }

    return result;
}

void Matrix::add_to_vector(std::vector<value_type>& a, const std::vector<value_type>& b) {
    if (a.size() != b.size()) {
        throw std::runtime_error("add_to_vector: size mismatch");
    }

    for (size_type i = 0; i < a.size(); ++i) {
        a[i] += b[i];
    }
}

std::ostream& operator<<(std::ostream& os, const Matrix& m) {
    for (Matrix::size_type i = 0; i < m.rows_; ++i) {
        for (Matrix::size_type j = 0; j < m.cols_; ++j) {
            os << m.data[i * m.cols_ + j] << ' ';
        }
        os << '\n';
    }

    return os;
}

std::istream& operator>>(std::istream& is, Matrix& m) {
    for (auto& val : m.data) {
        is >> val;
        if (!is) {
            break;
        }
    }

    return is;
}
