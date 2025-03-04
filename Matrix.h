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
	static void MultiT(const Matrix& m1, const double* neuron, int n, double* c);
	double& operator ()(int i, int j);
	friend std::ostream& operator << (std::ostream& os, const Matrix& m);
	friend std::istream& operator >> (std::istream is, Matrix& m);
};