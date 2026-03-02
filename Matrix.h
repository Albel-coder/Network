#pragma once
#include <iostream>
#include <vector>
#include <cstddef>

class Matrix
{
private:
	std::vector<double> data;
	std::size_t rows = 0;
	std::size_t cols = 0;

public:
	Matrix() = default;
	Matrix(std::size_t rows, std::size_t cols, double init = double{});
	Matrix(const Matrix&) = default;
	Matrix(Matrix&&) = default;

	Matrix& operator=(const Matrix&) = default;
	Matrix& operator=(Matrix&&) = default;

	double& operator()(std::size_t i, std::size_t j);
	const double& operator()(std::size_t i, std::size_t j) const;

	std::size_t rows() const noexcept { return rows; }
	std::size_t cols() const noexcept { return cols; }

	void random();

	std::vector<double> multiply(const std::vector<double>& vector) const;

	std::vector<double> multiplyTransposed(const std::vector<double>& vector) const;

	static void addToVector(std::vector<double>& firstVector, const std::vector<double>& secondVector);

	friend std::ostream& operator<<(std::ostream& output, const Matrix& matrix);
	friend std::istream& operator>>(std::istream& input, Matrix& matrix);
};
