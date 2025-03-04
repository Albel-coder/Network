#pragma once
#include <iostream>
enum activate_function { sigmoid = 1, ReLU, thx };

class ActivateFunction
{
	activate_function actFunc;
public:
	void set();
	void use(double* value, unsigned short n);
	void Derivate(double* value, unsigned short n);
	double Derivate(double value);
};