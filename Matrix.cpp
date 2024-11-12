#include "Matrix.h"
void Matrix::Initialization(unsigned short row, unsigned short column)
{
	this->row = row; this->column = column;
	matrix = new double* [row];
	for (int i = 0; i < row; i++) { matrix[i] = new double[column]; }

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
		{
			matrix[i][j] = 0;
		}
	}
}
void Matrix::Rand()
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
		{
			matrix[i][j] = ((rand() % 100) * 0.05 / (column + 45));
		}
	}
}
void Matrix::Multi(const Matrix& m1, const double* neuron, unsigned short n, double* c)
{
	if (m1.column != n) { throw std::runtime_error("matrix multiplication error \n"); }

	for (int i = 0; i < m1.row; ++i)
	{
		double tmp = 0;
		for (int j = 0; j < m1.column; ++j)
		{
			tmp += m1.matrix[i][j] * neuron[j];
		}
		c[i] = tmp;
	}
}
void Matrix::SumVector(double* a, const double* b, unsigned short n)
{
	for (int i = 0; i < n; i++)
	{
		a[i] += b[i];
	}
}