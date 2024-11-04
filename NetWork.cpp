#include "NetWork.h"
void NetWork::Initialization(data_NetWork data)
{
	actFunc.set();
	srand(time(0));
	Layer = data.Layer;
	size = new unsigned short[Layer];
	for (int i = 0; i < Layer; i++)
	{
		size[i] = data.size[i];
	}

	weights = new Matrix[Layer - 1];
	bios = new double* [Layer - 1];
	for (int i = 0; i < Layer - 1; i++)
	{
		weights[i].Initialization(size[i + 1], size[i]);
		bios[i] = new double[size[i + 1]];
		weights[i].Rand();
		for (int j = 0; j < size[i + 1]; j++)
		{
			bios[i][j] = ((rand() % 70)) * 0.05 / (size[i] + 20);
		}
	}
	neurons_val = new double* [Layer]; neuron_error = new double* [Layer];
	for (int i = 0; i < Layer; i++)
	{
		neurons_val[i] = new double[size[i]]; neuron_error[i] = new double[size[i]];
	}
	neuron_bios_value = new double[Layer - 1];
	for (int i = 0; i < Layer; i++)
	{
		neuron_bios_value[i] = 1;
	}
}
void NetWork::printConfig()
{
	for (int i = 0; i < 100; i++)
	{
		cout << "*";
	}
	cout << "\nNetWork has" << Layer << " Layers\nSIZE[]: ";
	for (int i = 0; i < Layer; i++)
	{
		cout << size[i] << " ";
	}
	for (int i = 0; i < 100; i++)
	{
		cout << "*";
	}
	cout << "\n";
}
void NetWork::Input(double* values)
{
	for (int i = 0; i < size[0]; i++)
	{
		neurons_val[0][i] = values[i];
	}
}
double NetWork::ForwardFeed()
{
	for (int k = 1; k < Layer; k++)
	{
		Matrix::Multi(weights[k - 1], neurons_val[k - 1], size[k - 1], neurons_val[k]);
		Matrix::SumVector(neurons_val[k], bios[k - 1], size[k]);
		actFunc.use(neurons_val[k], size[k]);
	}
	int predict = SearchMaxIndex(neurons_val[Layer - 1]);
	return predict;
}
int NetWork::SearchMaxIndex(double* value)
{
	double max = value[0];
	int prediction = 0;
	double tmp;
	for (int i = 0; i < size[Layer - 1]; i++)
	{
		tmp = value[i];
		if (tmp > max)
		{
			prediction = i;
			max = tmp;
		}
	}
	return prediction;
}
void NetWork::PrintValues(int Layer)
{
	for (int i = 0; i < size[Layer]; i++)
	{
		cout << i << " " << neurons_val[Layer][i] << "\n";
	}
}
