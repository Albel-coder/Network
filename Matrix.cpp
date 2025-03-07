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
void Matrix::Multi(const Matrix& m1, const double* neuron, int n, double* c)
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
void Matrix::SumVector(double* a, const double* b, int n)
{
	for (int i = 0; i < n; i++)
	{
		a[i] += b[i];
	}
}

void Matrix::MultiT(const Matrix& m1, const double* neuron, int n, double* c)
{
	if (m1.row != n)
	{
		throw std::runtime_error("Error multi \n");
	}

	double tmp = 0;
	for (int i = 0; i < m1.column; ++i)
	{
		for (int j = 0; j < m1.row; ++j)
		{
			tmp += m1.matrix[j][i] * neuron[j];
		}
		c[i] = tmp;
	}
}

double& Matrix::operator()(int i, int j)
{
	return matrix[i][j];
}

std::ostream& operator<<(std::ostream& os, const Matrix& m)
{
	for (int i = 0; i < m.row; ++i)
	{
		for (int j = 0; j < m.column; j++)
		{
			os << m.matrix[i][j] << " ";
		}
	}
	return os;
}

std::istream& operator>>(std::istream is, Matrix& m)
{
	for (int i = 0; i < m.row; ++i)
	{
		for (int j = 0; j < m.column; j++)
		{
			is >> m.matrix[i][j];
		}
	}
	return is;
}