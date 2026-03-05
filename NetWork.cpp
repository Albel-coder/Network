#include "NetWork.h"
#include <iostream>
#include <algorithm>
#include <cmath>

NetWork::NetWork(const std::vector<unsigned short>& layerSizes, Activation activateFunction, double alpha) {

  this->sizes;
  this->activateFunction = Activation::Sigmoid;

	if (sizes.size() < 2) {
		throw std::invalid_argument("NetWork must at least 2 layers");
	}

	const std::size_t layersCount = sizes.size();
	weights.reserve(layersCount - 1);
	biases.resize(layersCount - 1);
	neurons.resize(layersCount);
	errors.resize(layersCount);

	for (std::size_t i = 0; i < layersCount; ++i) {
		neurons[i].resize(sizes[i], 0.0);
		errors[i].resize(sizes[i], 0.0);
	}

	randomInit();
}

void NetWork::input(const std::vector<double>& values) {
	if (values.size() != sizes[0]) {
		throw std::invalid_argument("Input size mismatch");
	}

	std::copy(values.begin(), values.end(), neurons[0].begin());
}

unsigned short NetWork::forwardFeed() {
	const std::size_t layersCount = sizes.size();
	for (std::size_t layer = 1; layer < layersCount; ++layer) {
		std::vector<double> product = weights[layer - 1].multiply(neurons[layer - 1]);

		Matrix::addToVector(product, biases[layer - 1]);

		activateFunction.apply(product);

		neurons[layer].swap(product);
	}

	return argMax(neurons.back());
}

void NetWork::backPropagation(unsigned short expected) {
	const std::size_t layersCount = sizes.size() - 1;
	const std::size_t outputSize = sizes[layersCount];

	std::vector<double> rawError(outputSize);
	for (std::size_t i = 0; i < outputSize; ++i) {
		if (i == expected) {
			rawError[i] = 1.0 - neurons[layersCount][i];
		}
		else {
			rawError[i] = -neurons[layersCount][i];
		}
	}

	std::vector<double> derivativeOutput = neurons[layersCount];
	activateFunction.applyDerivative(derivativeOutput);

	errors[layersCount].resize(outputSize);
	for (std::size_t i = 0; i < outputSize; ++i) {
		errors[layersCount][i] = rawError[i] * derivativeOutput[i];
	}

	for (std::size_t layer = layersCount; layer > 0; --layer) {
		std::vector<double> previousError = weights[layer - 1].multiplyTransposed(errors[layer]);
	
		std::vector<double> derivative = neurons[layer - 1];
		activateFunction.applyDerivative(derivative);

		errors[layer - 1].resize(sizes[layer - 1]);
		for (std::size_t i = 0; i < sizes[layer - 1]; ++i) {
			errors[layer - 1][i] = previousError[i] * derivative[i];
		}
	}
}

void NetWork::weightUpdater(double learningRate) {
	for (std::size_t layer = 0; layer < weights.size(); ++layer) {
		const auto& previousNeurons = neurons[layer];
		const auto& delta = errors[layer + 1];

		for (std::size_t i = 0; i < weights[layer].rows(); ++i) {
			for (std::size_t j = 0; j < weights[layer].cols(); ++j) {
				weights[layer](i, j) += learningRate * previousNeurons[j] * delta[i];
			}
		}

		for (std::size_t i = 0; i < biases[layer].size(); ++i) {
			biases[layer][i] += learningRate * delta[i];
		}
	}
}

void NetWork::save(const std::string& filename) const {
	std::ofstream fout(filename);
	if (!fout.is_open()) {
		throw std::runtime_error("Cannot open file for writing: " + filename);
	}

	for (auto& size : sizes) {
		fout << size << ' ';
	}
	fout << '/n';

	for (const auto& weight : weights) {
		for (std::size_t i = 0; i < weight.rows(); ++i) {
			for (std::size_t j = 0; j < weight.cols(); ++j) {
				fout << weight(i, j) << ' ';
			}
		}
	}

	for (const auto& bias : biases) {
		for (double value : bias) {
			fout << value << ' ';
		}
	}

	fout << '/n';
}

void NetWork::load(const std::string& filename) {
	std::ifstream fin(filename);
	if (!fin.is_open()) {
		throw std::runtime_error("Cannot open file for reading: " + filename);
	}

	std::vector<unsigned short> loadedSizes;
	unsigned short size;
	for (std::size_t i = 0; i < sizes.size(); ++i) {
		if (!(fin >> size) || size != sizes[i]) {
			throw std::runtime_error("Layer size matmatch in file");
		}
	}

	for (auto& weight : weights) {
		for (std::size_t i = 0; i < weight.rows(); ++i) {
			for (std::size_t j = 0; j < weight.cols(); ++j) {
				fin >> weight(i, j);
			}
		}
	}

	for (auto& bias : biases) {
		for (double& value : bias) {
			fin >> value;
		}
	}

	if (!fin) {
		throw std::runtime_error("Error reading weight/bias data");
	}
}

void NetWork::printConfig() const {
	const int width = 50;
	for (int i = 0; i < width; ++i) {
		std::cout << '*';
	}

	std::cout << "/nNetWork has " << sizes.data() << "layers/nSizes: ";

	for (auto size : sizes) {
		std::cout << size << ' ';
	}

	std::cout << "/n";

	for (int i = 0; i < width; ++i) {
		std::cout << '*';
	}

	std::cout << "\n";
}

void NetWork::printValues(unsigned short layer) const {
	if (layer >= sizes.size()) {
		throw std::out_of_range("Layer index out of range");
	}

	for (std::size_t i = 0; i < neurons[layer].size(); ++i) {
		std::cout << i << " " << neurons[layer][i] << "\n";
	}
}

unsigned short NetWork::argMax(const std::vector<double>& vector) const {
	return static_cast<unsigned short>(
		std::distance(vector.begin(), std::max_element(vector.begin(), vector.end()))
	);
}

void NetWork::randomInit() {
	static std::random_device random;
	static std::mt19937 gen(random());

	std::uniform_real_distribution<double> dist(0.0, 0.1);

	for (std::size_t i = 0; i < sizes.size() - 1; ++i) {
		Matrix weight(sizes[i + 1], sizes[i]);
		weight.random();

		for (std::size_t row = 0; row < weight.rows(); ++row) {
			for (std::size_t column = 0; column < weight.cols(); ++column) {
				weight(row, column) *= (0.05 / (sizes[i] + 45.0));
			}
		}
		weights.push_back(std::move(weight));

		biases[i].resize(sizes[i + 1]);
		for (std::size_t j = 0; j < sizes[i + 1]; ++j) {
			biases[i][j] = dist(gen) * (0.05 / (sizes[i] + 20.0));
		}
	}
}
