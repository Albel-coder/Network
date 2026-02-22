<<<<<<< HEAD
#pragma once
#include "ActivateFunction.h"
#include "Matrix.h"
#include <fstream>
using namespace std;
struct data_NetWork
{
	int Layer;
	int* size;
};
class NetWork
{
	unsigned short Layer;
	unsigned short* size;
	ActivateFunction actFunc;
	Matrix* weights;
	double** bios;
	double** neurons_val, ** neuron_error;
	double* neuron_bios_value;
public:
	void Initialization(data_NetWork data);
	void printConfig();
	void Input(double* values);

	double ForwardFeed();
	int SearchMaxIndex(double* value);
	void PrintValues(int Layer);

	void BackPropogation(double expect);
	void WeightsUpdater(double lr);

	void SaveWeight();
	void ReadWeight();
=======
#pragma once
#include "ActivateFunction.h"
#include "Matrix.h"
#include <fstream>
using namespace std;
struct data_NetWork
{
	int Layer;
	int* size;
};
class NetWork
{
	unsigned short Layer;
	unsigned short* size;
	ActivateFunction actFunc;
	Matrix* weights;
	double** bios;
	double** neurons_val, ** neuron_error;
	double* neuron_bios_value;
public:
	void Initialization(data_NetWork data);
	void printConfig();
	void Input(double* values);

	double ForwardFeed();
	int SearchMaxIndex(double* value);
	void PrintValues(int Layer);

	void BackPropogation(double expect);
	void WeightsUpdater(double lr);

	void SaveWeight();
	void ReadWeight();
>>>>>>> a0871e0 (fixed)
};