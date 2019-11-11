#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <random>
#include <iomanip>

using namespace std;
using NeuralNet = std::vector<std::vector<std::vector<float>>>;
using DataType = int;
using DataSet = std::vector<std::vector<std::vector<DataType>>>;

float relu (float threshold, float input) {
  if(input - threshold <= 0) return 0;
  else return input - threshold;
}

void network_check (NeuralNet& nn) {
  for(size_t i=0; i<nn.size(); ++i) {
    for(size_t j=0; j<nn[i].size(); ++j) {
      for(size_t k=0; k<nn[i][j].size(); ++k) {
        std::cout << nn[i][j][k] << " ";
      }
      std::cout << std::endl;
    }
  }
}

void network_size_check (NeuralNet& nn) {
  for(size_t i=0; i<nn.size(); ++i) {
    std::cout << nn[i].size() << std::endl;
  }
}

void dataset_check (DataSet& dataset) {
  for(const auto& vv : dataset) {
    for(const auto& v : vv) {
      for(const auto& e : v) {
        cout << setw(3) << e << " ";
      }
      cout << endl;
    }
    cout << endl;
  }
  return;
}

void init_neuralnet (NeuralNet& nn, vector<size_t> sizes) {
  size_t nn_num = 0;
  for(size_t layer=0; layer<sizes.size(); ++layer) nn_num += sizes[layer] * sizes[layer+1];
  nn.resize(0);
  nn.reserve(nn_num);
  for(size_t layer=0; layer<sizes.size(); ++layer) {
    if(layer==sizes.size()-1) return;
    nn.push_back(vector<vector<float>>(sizes[layer]));
    for(size_t pre = 0; pre < sizes[layer]; ++pre) {
      nn[layer][pre].resize(sizes[layer+1]);
    }
  }
}

template<typename T> float calculate_accuracy(NeuralNet& nn, DataSet& test) {
  float accuracy = 0.0;
  return accuracy;
}

int estimate(NeuralNet& nn, vector<int>& estimate_data) {
  //TODO: this function is estimate from input on network
  int estimated_class = 0;
  return estimated_class;
}

void back_propagation(NeuralNet& nn, DataSet& test) {
  std::default_random_engine generator;
  std::normal_distribution<float> distribution(0.0,0.1);
  // generate normal distribution : `distribution(generator)`
  for(size_t i=0; i<nn.size(); ++i) {
    for(size_t j=0; j<nn[i].size(); ++j) {
      for(size_t k=0; k<nn[i][j].size(); ++k) {
        float d = distribution(generator);
      }
    }
  }
}

void learn(NeuralNet& nn, DataSet& test) {
//  auto learning_process[](){
//    back_propagation(nn, test);
//  }
//  for_progress(100, learning_process);
  for(int i=0; i<100; ++i) {
    back_propagation(nn, test);
  }
  return ;
}

void load_data (std::string dataset_path, DataSet& train, DataSet& test) {
//TODO: develop function for load dataset. load PNG to `train`, `test`
  return ;
}

int main() {
//  vector<size_t> sizes = {256, 128, 64, 10};
  vector<size_t> sizes = {128, 64, 10};
  NeuralNet nn;
//  DataSet test;
  init_neuralnet(nn, sizes);
  vector<vector<float>> v_for_learn;
//  learn(nn, test);
//  network_check(nn);


  DataSet train(10, std::vector<std::vector<int>>(28, std::vector<int>(28)));
  for(int i=0; i<10; ++i) {
    fstream file("dataset/train/" + to_string(i) + ".csv");
    string line;
    size_t n=0;
    while (getline( file, line,'\n'))	{
      istringstream templine(line);
      string data;
      size_t m = 0;
      while (getline( templine, data,',')) {
        cout << i << " " << n << " " << m << " " << endl;
        train[i][n][m] = atoi(data.c_str());
        m++;
      }
      n++;
    }
    file.close();
  }
  dataset_check(train);

  return 0;
}
