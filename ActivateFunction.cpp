#include "ActivateFunction.h"

ActivateFunction::ActivateFunction(Activation type, double alpha)
{
   this->type = type;
   this->alpha = alpha;
}

void ActivateFunction::setType(Activation type) noexcept {
	this->type = type;
}

void ActivateFunction::setAlpha(double alpha) noexcept {
	this->alpha = alpha;
}

void ActivateFunction::apply(std::vector<double>& values) const {
	for (double& x : values) {
		x = operator()(x);
	}
}

void ActivateFunction::applyDerivative(std::vector<double>& values) const {
	for (double& y : values) {
		switch (type)
		{
		case Activation::Sigmoid:
			y = y * (1.0 - y);
			break;
		case Activation::ReLU:
			if (y < 0.0 || y > 1.0) {
				y = alpha;
			}
			else {
				y = 1.0;
			}
			break;
		case Activation::Tanh:
			if (y < 0.0) {
				y = alpha * (1.0 - y * y);
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
	switch (type)
	{
	case Activation::Sigmoid:
		return 1.0 / (1.0 + std::exp(-x));
	case Activation::ReLU:
		if (x < 0.0) {
			return x * alpha;
		}
		else if (x <= 1.0) {
			return x;
		}
		else {
			return 1.0 + alpha * (x - 1.0);
		}
	case Activation::Tanh:
	{
		double ex = std::exp(x);
		double emx = std::exp(-x);
		double tanhValue = (ex - emx) / (ex + emx);
		if (x < 0.0) {
			return alpha * tanhValue;
		}
		else {
			return tanhValue;
		}
	}
	default:
		return x;
	}
}

double ActivateFunction::derivative(double x) const noexcept {
	switch (type)
	{
	case Activation::Sigmoid:
		double sigmoid = 1.0 / (1.0 + std::exp(-x));
		return sigmoid * (1.0 - sigmoid);
	case Activation::ReLU:
		if (x < 0.0 || x > 1.0) {
			return alpha;
		}
		else {
			return 1.0;
		}
	case Activation::Tanh:
		double th = operator()(x);
		if (x < 0.0) {
			double tanhX = th / alpha;
			return alpha * (1.0 - tanhX * tanhX);
		}
		else {
			return 1.0 - th * th;
		}
	default:
		return 0.0;
	}
}
