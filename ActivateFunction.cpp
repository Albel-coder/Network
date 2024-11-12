#include "ActivateFunction.h"
void ActivateFunction::set()
{
	std::cout << "Set activate function\n0 - sigmoid \n1 - ReLU \n2 - th(x) \n";
	short tmp;
	std::cin >> tmp;
	switch (tmp)
	{
	case sigmoid:
		actFunc = sigmoid;
		break;
	case ReLU:
		actFunc = ReLU;
		break;
	case thx:
		actFunc = thx;
		break;
	default:
		throw std::runtime_error("Error read activate function");
		break;
	}
}
void ActivateFunction::use(double* value, unsigned short n)
{
	switch (actFunc)
	{
	case activate_function::sigmoid:
		for (int i = 0; i < n; i++) { value[i] = 1 / (1 + exp(-value[i])); }
		break;
	case activate_function::ReLU:
		for (int i = 0; i < n; i++)
		{
			if (value[i] < 0) { value[i] *= 0.01; }
			else if (value[i] > 1) { value[i] = 1.0 + 0.01 * (value[i] - 1); }
		}
	case activate_function::thx:
		for (int i = 0; i < n; i++)
		{
			if (value[i] < 0)
			{
				value[i] = 0.01 * (exp(value[i]) - exp(-value[i])) / (exp(value[i]) + exp(-value[i]));
			}
			else
			{
				value[i] = (exp(value[i]) - exp(-value[i])) / (exp(value[i]) + exp(-value[i]));
			}
		}
		break;
	default:
		throw std::runtime_error("Error activate function\n");
		break;
	}
}
void ActivateFunction::Derivate(double* value, unsigned short n)
{
	switch (actFunc)
	{
	case activate_function::sigmoid:
		for (int i = 0; i < n; i++)
		{
			value[i] = value[i] * (1 - value[i]);
		}
		break;
	case activate_function::ReLU:
		for (int i = 0; i < n; i++)
		{
			if (value[i] < 0 || value[i] > 1) { value[i] = 0.01; }
			else { value[i] = 1; }
		}
		break;
	case activate_function::thx:
		for (int i = 0; i < n; i++)
		{
			if (value[i] < 0) { value[i] = 0.01 * (1 - value[i] * value[i]); }
			else { value[i] = 1 - value[i] * value[i]; }
		}
		break;
	default:
		throw std::runtime_error("Error activate function derivate\n");
		break;
	}
}
