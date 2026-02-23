#include "ActivateFunction.h"

ActivateFunction::ActivateFunction(Activation type, double alpha)
	: type_(type), alpha_(alpha)
{}

void ActivateFunction::setType(Activation type) noexcept {
	type_ = type;
}

void ActivateFunction::setAlpha(double alpha) noexcept {
	alpha_ = alpha;
}

void ActivateFunction::apply(std::vector<double>& values) const {
	for (double& x : values) {
		x = operator()(x);
	}
}

void ActivateFunction::applyDerivative(std::vector<double>& values) const {
	for (double& y : values) {
		switch (type_)
		{
		case Activation::Sigmoid:
			y = y * (1.0 - y);
			break;
		case Activation::ReLU:
			if (y < 0.0 || y > 1.0) {
				y = alpha_;
			}
			else {
				y = 1.0;
			}
			break;
		case Activation::Tanh:
			if (y < 0.0) {
				y = alpha_ * (1.0 - y * y);
			}
			else {
				y = 1.0 - y * y;
			}
			break;
		default:
			break;
		}
	}
}

double ActivateFunction::operator()(double x) const noexcept {
	switch (type_)
	{
	case Activation::Sigmoid:
		return 1.0 / (1.0 + std::exp(-x));
	case Activation::ReLU:
		if (x < 0.0) {
			return x * alpha_;
		}
		else if (x <= 1.0) {
			return x;
		}
		else {
			return 1.0 + alpha_ * (x - 1.0);
		}
	case Activation::Tanh:
	{
		double ex = std::exp(x);
		double emx = std::exp(-x);
		double tanh_val = (ex - emx) / (ex + emx);
		if (x < 0.0) {
			return alpha_ * tanh_val;
		}
		else {
			return tanh_val;
		}
	}
	default:
		return x;
	}
}

double ActivateFunction::derivative(double x) const noexcept {
	switch (type_)
	{
	case Activation::Sigmoid:
		double s = 1.0 / (1.0 + std::exp(-x));
		return s * (1.0 - s);
	case Activation::ReLU:
		if (x < 0.0 || x > 1.0) {
			return alpha_;
		}
		else {
			return 1.0;
		}
	case Activation::Tanh:
		double th = operator()(x);
		if (x < 0.0) {
			double tanh_x = th / alpha_;
			return alpha_ * (1.0 - tanh_x * tanh_x);
		}
		else {
			return 1.0 - th * th;
		}
	default:
		return 0.0;
	}
}
