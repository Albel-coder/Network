# Documentation for neural network in C++

## Description
The library implements a neural network for classification tasks (e.g. MNIST handwritten digit recognition). Supports network architecture configuration, training using backpropagation, saving and loading weights.

---

## Requirements
- C++17 compiler (MSVC, GCC, Clang support)
- Standard libraries: <vector>, <cmath>, <fstream>
- Build system: CMake (optional)

---
## Network Configuration
Create a Config.txt file in the format:
NetWork <Number_of_Layers>
<Layer_Size_0> <Layer_Size_1> ... <Layer_Size_N>
Example for network 784-128-10:
NetWork 3
784 128 10
---

## Usage
### 1. Initializing the network
#include "NetWork.h"

int main() {
NetWork network;
data_NetWork config = ReadDataNetWork("Config.txt"); // Loading the configuration
network.Initialization(config);
network.printConfig(); // Printing information about the network
return 0;
}


### 2. Network training
data_info* train_data;
int examples;
train_data = ReadData("lib_MNIST_edit.csv", config, examples);

for (int epoch = 0; epoch < 100; ++epoch) {
    for (int i = 0; i < examples; ++i) {
        network.Input(train_data[i].pixels);
        double prediction = network.ForwardFeed();
        if (prediction != train_data[i].digit) {
            network.BackPropogation(train_data[i].digit);
            network.WeightsUpdater(0.15 * exp(-epoch / 20.));
        }
    }
    network.SaveWeight(); // Save weights after epoch
}


### 3. Prediction
data_info* test_data;
int test_examples;
test_data = ReadData("lib_10k.csv", config, test_examples);

int correct = 0;
for (int i = 0; i < test_examples; ++i) {
    network.Input(test_data[i].pixels);
    double prediction = network.ForwardFeed();
    if (prediction == test_data[i].digit) correct++;
}
std::cout << "Accuracy: " << (correct * 100.0 / test_examples) << "%\n";
---

## Integration into projects
1. Include header files:

#include "NetWork.h"
#include "Matrix.h"
#include "ActivateFunction.h"

2. Use methods:
- Input(double* values) — transfer input data.
- ForwardFeed() — receive prediction.
- ReadWeight() / SaveWeight() — work with weights.

---
## Data format
- Training data (lib_MNIST_edit.csv):

Example: <Label> <Pixel_0> <Pixel_1> ... <Pixel_783>

- Pixels should be normalized (values ​​0.0–1.0).

---

## Setting up activation functions
By default, ReLU is used. To change:
ActivateFunction func;
func.setType(ActivationType::Sigmoid); // Before initializing the network
---

## Tips
- To speed up training:
- Use adaptive learning rate (example in code: 0.15 * exp(-epoch / 20.)).
- Normalize input data.
- For debugging:
- Enable printConfig() and PrintValues(int layer) output.

---
