#pragma once

#include <vector>
#include <cmath>
#include <stdexcept>

enum class Activation {
	Sigmoid,
	ReLU,
	Tanh
};

class ActivateFunction {
public:

	explicit ActivateFunction(Activation type = Activation::Sigmoid, double alpha);

	void setType(Activation type) noexcept;
	void setAlpha(double alpha) noexcept;

	void apply(std::vector<double>& values) const;

	void applyDerivative(std::vector<double>& values) const;

	double operator()(double x) const noexcept;
	double derivative(double x) const noexcept;

private:
	Activation type_;
	double alpha_;
};