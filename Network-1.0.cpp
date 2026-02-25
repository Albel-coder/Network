#include "NetWork.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>

using namespace std;

// Structure for storing one example (label + pixels)
struct Example {
    int label;
    vector<double> pixels;
};

// Reading network configuration from file
vector<unsigned short> readConfig(const string& path) {
    ifstream fin(path);
    if (!fin.is_open())
        throw runtime_error("Cannot open config file: " + path);

    string token;
    fin >> token;
    if (token != "NetWork")
        throw runtime_error("Invalid config format: expected 'NetWork'");

    int layers;
    fin >> layers;
    if (layers < 2)
        throw runtime_error("Network must have at least 2 layers");

    vector<unsigned short> sizes(layers);
    for (int i = 0; i < layers; ++i) {
        int s;
        fin >> s;
        if (s <= 0)
            throw runtime_error("Invalid layer size");
        sizes[i] = static_cast<unsigned short>(s);
    }
    return sizes;
}

// Reading data (training or testing) from a CSV file
vector<Example> readData(const string& path, size_t inputSize) {
    ifstream fin(path);
    if (!fin.is_open())
        throw runtime_error("Cannot open data file: " + path);

    string token;
    fin >> token;
    if (token != "Examples")
        throw runtime_error("Invalid data format: expected 'Examples'");

    int numExamples;
    fin >> numExamples;
    if (numExamples <= 0)
        throw runtime_error("No examples found");

    vector<Example> data;
    data.reserve(numExamples);

    for (int i = 0; i < numExamples; ++i) {
        Example ex;
        fin >> ex.label;

        ex.pixels.resize(inputSize);
        for (size_t j = 0; j < inputSize; ++j) {
            double p;
            fin >> p;
            // Normalize pixels to the range [0,1]
            ex.pixels[j] = p / 255.0;
        }
        data.push_back(move(ex));
    }
    cout << "Loaded " << data.size() << " examples from " << path << "\n";
    return data;
}

// Calculate the accuracy of the network on the dataset
double evaluate(NetWork& net, const vector<Example>& dataset) {
    int correct = 0;
    for (const auto& ex : dataset) {
        net.input(ex.pixels);
        unsigned short pred = net.forwardFeed();
        if (pred == ex.label)
            ++correct;
    }
    return 100.0 * correct / dataset.size();
}

int main() {
    try {
        // Load the network configuration
        auto sizes = readConfig("Config.txt");
        NetWork net(sizes, Activation::Sigmoid);  // you can change the activation type

        net.printConfig();

        // Load training and test data
        size_t inputSize = sizes[0];
        auto trainData = readData("lib_MNIST_edit.csv", inputSize);
        auto testData = readData("lib_10k.csv", inputSize);

        bool repeat = true;
        while (repeat) {
            cout << "\nTrain new network? (1 - yes, 0 - load weights): ";
            int choice;
            cin >> choice;

            if (choice == 1) {
                // Studing
                int epochs;
                double lr;
                cout << "Enter number of epochs: ";
                cin >> epochs;
                cout << "Enter learning rate: ";
                cin >> lr;

                auto startTime = chrono::steady_clock::now();

                for (int epoch = 1; epoch <= epochs; ++epoch) {
                    // Shuffle the data before each epoch (for stochasticity)
                    shuffle(trainData.begin(), trainData.end(), mt19937(random_device()()));

                    for (const auto& ex : trainData) {
                        net.input(ex.pixels);
                        net.forwardFeed();
                        net.backPropagation(ex.label);
                        //net.weightsUpdater(lr);
                    }
                    // Evaluation on the test sample after the epoch
                    double acc = evaluate(net, testData);
                    auto now = chrono::steady_clock::now();
                    chrono::duration<double> elapsed = now - startTime;
                    cout << "Epoch " << epoch << "/" << epochs
                        << " | Test accuracy: " << acc << "%"
                        << " | Time: " << elapsed.count() << " s\n";
                }

                // Save weights after training
                net.save("Weights.txt");
                cout << "Weights saved to Weights.txt\n";
            }
            else {
                // Loading ready-made weights
                net.load("Weights.txt");
                cout << "Weights loaded from Weights.txt\n";
            }

            // Testing
            cout << "Run test on test set? (1 - yes, 0 - skip): ";
            int testChoice;
            cin >> testChoice;
            if (testChoice == 1) {
                double acc = evaluate(net, testData);
                cout << "Final test accuracy: " << acc << "%\n";
            }

            cout << "Repeat program? (1 - yes, 0 - exit): ";
            cin >> repeat;
        }
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}