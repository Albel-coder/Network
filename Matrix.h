#pragma once
#include <iostream>
class Matrix
{
	double** matrix;
	unsigned short row, column;
public:
	void Initialization(unsigned short row, unsigned short column);
	void Rand();
	static void Multi(const Matrix& m, const double* b, unsigned short n, double* c);
	static void SumVector(double* a, const double* b, unsigned short n);
};

