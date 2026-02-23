#pragma once

#include "Matrix.h"
#include "ActivateFunction.h"
#include <vector>
#include <fstream>
#include <stdexcept>
#include <random>

class NetWork {
public:
	NetWork(const std::vector<unsigned short>& layerSizes,
		Activation activateFunction = Activation::Sigmoid,
		double alpha = 0.01);

	NetWork(const NetWork&) = delete;
	NetWork& operator=(const NetWork&) = delete;

	NetWork(NetWork&&) = default;
	NetWork& operator=(NetWork&&) = default;

	void input(const std::vector<double>& values);

	unsigned short forwardFeed();

	void backPropagation(unsigned short expected);

	void weightUpdater(double learningRate);

	void save(const std::string& filename) const;

	void load(const std::string& filename);

	void printConfig() const;

	void printValues(unsigned short layer) const;

private:
	std::vector<unsigned short> sizes_;
	ActivateFunction activateFunction_;

	std::vector<Matrix> weights_;
	std::vector<std::vector<double>> biases_;

	std::vector<std::vector<double>> neurons_;
	std::vector<std::vector<double>> errors_;

	unsigned short argMax(const std::vector<double>& vector) const;

	void randomInit();
};