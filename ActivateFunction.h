#pragma once
#include <iostream>
enum activate_function { sigmoid = 1, ReLU, thx };

class ActivateFunction
{
	activate_function activateFunc;
public:
	void set();
	void use(double* value, unsigned short n);
	void UseDerivate(double* value, unsigned short n);
	double Derivate(double value);
};