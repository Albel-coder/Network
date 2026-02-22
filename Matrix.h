#pragma once
#include <iostream>
#include <vector>
#include <cstddef>

class Matrix {
public:
    using value_type = double;
    using size_type = std::size_t;

private:
    std::vector<value_type> data;
    size_type rows_ = 0;
    size_type cols_ = 0;

public:
    Matrix() = default;
    Matrix(size_type rows, size_type cols, value_type init = value_type{});
    Matrix(const Matrix&) = default;
    Matrix(Matrix&&) = default;

    Matrix& operator=(const Matrix&) = default;
    Matrix& operator=(Matrix&&) = default;

    value_type& operator()(size_type i, size_type j);
    const value_type& operator()(size_type i, size_type j) const;

    size_type rows() const noexcept { return rows_; }
    size_type cols() const noexcept { return cols_; }

    void random();

    std::vector<value_type> multiply(const std::vector<value_type>& vec) const;

    std::vector<value_type> multiply_transposed(const std::vector<value_type>& vec) const;

    static void add_to_vector(std::vector<value_type>& a, const std::vector<value_type>& b);

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m);
    friend std::istream& operator>>(std::istream& is, Matrix& m);
};
