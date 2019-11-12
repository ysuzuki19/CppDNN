#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <random>
#include <iomanip>

using namespace std;
const std::string DataPath = "./dataset";
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

void dataset_check (std::vector<int>& v) {
  for(const auto& e : v) {
    cout << setw(3) << e << " ";
  }
  cout << endl;
  return;
}

void dataset_check (std::vector<std::vector<int>>& image) {
  for(const auto& v : image) {
    for(const auto& e : v) {
      cout << setw(3) << e << " ";
    }
    cout << endl;
  }
  return;
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
  return;
}

void load_dataset (std::filesystem::path dataset_path, DataSet& x_train, std::vector<int>& y_train) {
  fstream file(dataset_path/"y.csv");
  string line;
  while (getline(file, line,'\n'))	{
    istringstream oneline(line);
    string data;
    while (getline( oneline, data,',')) {
      if(data == " ") break;
      y_train.push_back(stoi(data));
    }
  }
  size_t data_col = 0, data_row = 0;
  {
    fstream file(dataset_path/"0.csv");
    while (getline(file, line,'\n'))	{
      istringstream oneline(line);
      string data;
      data_row = 0;
      while (getline(oneline, data,',')) {
        if(data==" ") break;
        data_row++;
      }
      data_col++;
    }
    file.close();
  }
  x_train.resize(y_train.size(), std::vector<std::vector<int>>(data_col, std::vector<int>(data_row)));
  for(int i=0; i<y_train.size(); ++i) {
    fstream file(dataset_path/(to_string(i)+".csv"));
    size_t n = 0;
    while (getline(file, line,'\n'))	{
      istringstream oneline(line);
      string data;
      size_t m = 0;
      while (getline(oneline, data,',')) {
        if(data==" ") break;
        x_train[i][n][m] = stoi(data);
        m++;
      }
      n++;
    }
    file.close();
  }
  return;
}

int main() {
//  vector<size_t> sizes = {256, 128, 64, 10};
  vector<size_t> sizes = {128, 64, 10};
  NeuralNet nn;
//  DataSet test;
  init_neuralnet(nn, sizes);
//  vector<vector<float>> v_for_learn;
  vector<int> y_train;
  DataSet x_train;
  std::filesystem::path train_path = DataPath;
  train_path /= "train";
  load_dataset(DataPath + "/train", x_train, y_train);
  cout << y_train.size() << endl;
  cout << x_train.size() << endl;
//  learn(nn, test);
//  network_check(nn);

//  dataset_check(x_train[500]);
//  cout << x_train.size() << endl;

  return 0;
}
