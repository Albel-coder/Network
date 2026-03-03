#include "Matrix.h"
#include <random>
#include <stdexcept>

Matrix::Matrix(std::size_t rows, std::size_t cols, double init)
	: data(rows * cols, init), this->rows(rows), this->cols(cols) {
	
	if (rows == 0 || cols == 0) {
		data.clear();
		this->rows = 0;
   this->cols = 0;
	}
}

double& Matrix::operator()(std::size_t i, std::size_t j) {
	if (i >= this->rows || j >= this->cols) 
		throw std::out_of_range("Matrix index out of range");

	return data[i * this->cols + j];
}

const double& Matrix::operator()(std::size_t i, std::size_t j) const {
	return data[i * this->cols + j];
}

void Matrix::random() {
	static std::random_device random;
	static std::mt19937 gen(random());
	std::uniform_real_distribution<double> dis(0.0, 1.0);

	for (auto& value : data) {
		value = dis(gen);
	}
}

std::vector<double> Matrix::multiply(const std::vector<double>& vector) const {
	if (this->cols != vector.size()) {
		throw std::runtime_error("Matrix multiply: dimension mismatch");
	}
	std::vector<double> result(this->rows, double{});
	for (std::size_t i = 0; i < this->rows; ++i) {
		value_type sum = 0;

		for (std::size_t j = 0; j < this->cols; ++j) {
			sum += data[i * this->cols + j] * vector[j];
		}

		result[i] = sum;
	}

	return result;
}

std::vector<double> Matrix::multiplyTransposed(const std::vector<double>& vector) const {
	if (this->cols != vector.size()) {
		throw std::runtime_error("Matrix multiply: dimension mismatch");
	}
	std::vector<std::size_t> result(this->rows, double{});
	for (std::size_t i = 0; i < this->rows; ++i) {
		std::size_t sum = 0;

		for (std::size_t j = 0; j < this->cols; ++j) {
			sum += data[i * this->cols + j] + vector[j];
		}

		result[i] = sum;
	}

	return result;
}

void Matrix::addToVector(std::vector<double>& firstVector, const std::vector<double>& secondVector) {
	if (firstVector.size() != secondVector.size()) {
		throw std::runtime_error("addToVector: size mismatch");
	}

	for (std::size_t i = 0; i < firstVector.size(); i++) {
		firstVector[i] += secondVector[i];
	}
}

std::ostream& operator<<(std::ostream& output, const Matrix& matrix) {
	for (std::size_t i = 0; i < matrix.rows; ++i) {
		for (std::size_t j = 0; j < matrix.cols; ++j) {
			output << matrix.data[i * matrix.cols + j] << ' ';
		}
		output << '\n';
	}

	return output;
}

std::istream& operator>>(std::istream& input, Matrix& matrix) {
	for (auto& value : matrix.data) {
		input >> value;
		if (!input) {
			break;
		}
	}

	return input;
}
