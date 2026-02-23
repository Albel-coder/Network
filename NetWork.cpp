#include "NetWork.h"
#include <iostream>
#include <algorithm>
#include <cmath>

NetWork::NetWork(const std::vector<unsigned short>& layerSizes, Activation activateFunction, double alpha)
	: sizes_(layerSizes),
	activateFunction_(activateFunction, alpha) {

	if (sizes_.size() < 2) {
		throw std::invalid_argument("NetWork must at least 2 layers");
	}

	const size_t numLayers = sizes_.size();
	weights_.reserve(numLayers - 1);
	biases_.resize(numLayers - 1);
	neurons_.resize(numLayers);
	errors_.resize(numLayers);

	for (size_t i = 0; i < numLayers; ++i) {
		neurons_[i].resize(sizes_[i], 0.0);
		errors_[i].resize(sizes_[i], 0.0);
	}

	randomInit();
}

void NetWork::input(const std::vector<double>& values) {
	if (values.size() != sizes_[0]) {
		throw std::invalid_argument("Input size mismatch");
	}

	std::copy(values.begin(), values.end(), neurons_[0].begin());
}

unsigned short NetWork::forwardFeed() {
	const size_t numLayers = sizes_.size();
	for (size_t layer = 1; layer < numLayers; ++layer) {
		std::vector<double> product = weights_[layer - 1].multiply(neurons_[layer - 1]);

		Matrix::addToVector(product, biases_[layer - 1]);

		activateFunction_.apply(product);

		neurons_[layer].swap(product);
	}

	return argMax(neurons_.back());
}

void NetWork::backPropagation(unsigned short expected) {
	const size_t L = sizes_.size() - 1;
	const size_t outputSize = sizes_[L];

	std::vector<double> rawError(outputSize);
	for (size_t i = 0; i < outputSize; ++i) {
		if (i == expected) {
			rawError[i] = 1.0 - neurons_[L][i];
		}
		else {
			rawError[i] = -neurons_[L][i];
		}
	}

	std::vector<double> derivativeOutput = neurons_[L];
	activateFunction_.applyDerivative(derivativeOutput);

	errors_[L].resize(outputSize);
	for (size_t i = 0; i < outputSize; ++i) {
		errors_[L][i] = rawError[i] * derivativeOutput[i];
	}

	for (size_t layer = L; layer > 0; --layer) {
		std::vector<double> prevError = weights_[layer - 1].multiplyTransposed(errors_[layer]);
	
		std::vector<double> derivative = neurons_[layer - 1];
		activateFunction_.applyDerivative(derivative);

		errors_[layer - 1].resize(sizes_[layer - 1]);
		for (size_t i = 0; i < sizes_[layer - 1]; ++i) {
			errors_[layer - 1][i] = prevError[i] * derivative[i];
		}
	}
}

void NetWork::weightUpdater(double learningRate) {
	for (size_t layer = 0; layer < weights_.size(); ++layer) {
		const auto& prevNeurons = neurons_[layer];
		const auto& delta = errors_[layer + 1];

		for (size_t i = 0; i < weights_[layer].rows(); ++i) {
			for (size_t j = 0; j < weights_[layer].cols(); ++j) {
				weights_[layer](i, j) += learningRate * prevNeurons[j] * delta[i];
			}
		}

		for (size_t i = 0; i < biases_[layer].size(); ++i) {
			biases_[layer][i] += learningRate * delta[i];
		}
	}
}

void NetWork::save(const std::string& filename) const {
	std::ofstream fout(filename);
	if (!fout.is_open()) {
		throw std::runtime_error("Cannot open file for writing: " + filename);
	}

	for (auto& size : sizes_) {
		fout << size << ' ';
	}
	fout << '/n';

	for (const auto& weight : weights_) {
		for (size_t i = 0; i < weight.rows(); ++i) {
			for (size_t j = 0; j < weight.cols(); ++j) {
				fout << weight(i, j) << ' ';
			}
		}
	}

	for (const auto& bias : biases_) {
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
	for (size_t i = 0; i < sizes_.size(); ++i) {
		if (!(fin >> size) || size != sizes_[i]) {
			throw std::runtime_error("Layer size matmatch in file");
		}
	}

	for (auto& weight : weights_) {
		for (size_t i = 0; i < weight.rows(); ++i) {
			for (size_t j = 0; j < weight.cols(); ++j) {
				fin >> weight(i, j);
			}
		}
	}

	for (auto& bias : biases_) {
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

	std::cout << "/nNetWork has " << sizes_.data() << "layers/nSizes: ";

	for (auto size : sizes_) {
		std::cout << size << ' ';
	}

	std::cout << "/n";

	for (int i = 0; i < width; ++i) {
		std::cout << '*';
	}

	std::cout << "\n";
}

void NetWork::printValues(unsigned short layer) const {
	if (layer >= sizes_.size()) {
		throw std::out_of_range("Layer index out of range");
	}

	for (size_t i = 0; i < neurons_[layer].size(); ++i) {
		std::cout << i << " " << neurons_[layer][i] << "\n";
	}
}

unsigned short NetWork::argMax(const std::vector<double>& vector) const {
	return static_cast<unsigned short>(
		std::distance(vector.begin(), std::max_element(vector.begin(), vector.end()))
	);
}

void NetWork::randomInit() {
	static std::random_device rd;
	static std::mt19937 gen(rd());

	std::uniform_real_distribution<double> dist(0.0, 0.1);

	for (size_t i = 0; i < sizes_.size() - 1; ++i) {
		Matrix w(sizes_[i + 1], sizes_[i]);
		w.random();

		for (size_t r = 0; r < w.rows(); ++r) {
			for (size_t c = 0; c < w.cols(); ++c) {
				w(r, c) *= (0.05 / (sizes_[i] + 45.0));
			}
		}
		weights_.push_back(std::move(w));

		biases_[i].resize(sizes_[i + 1]);
		for (size_t j = 0; j < sizes_[i + 1]; ++j) {
			biases_[i][j] = dist(gen) * (0.05 / (sizes_[i] + 20.0));
		}
	}
}
