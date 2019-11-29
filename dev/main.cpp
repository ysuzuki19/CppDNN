#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <random>

#ifdef __APPLE__
  #include <filesystem>
  namespace fs = std::filesystem;
#elif __linux__
  #include <experimental/filesystem>
  namespace fs = std::experimental::filesystem;
#else
  #error "OS not supported."
#endif

#include <debug.hpp>
#include <dataset.hpp>
#include <neuralnet.hpp>

using namespace std;
const std::vector<std::string> DataPathVec {"..", "dataset"};
using DataType = int;
using NeuralNetType = std::vector<std::vector<std::vector<float>>>;
using ImageType = std::vector<std::vector<DataType>>;

void view_network_sizes (const NeuralNetType& nn) {
  for(const auto& layer : nn) {
    std::cout << layer.size() << std::endl;
  }
}

void view_network (const NeuralNetType& nn) {
  for(const auto& vv : nn) {
    for(const auto& v : vv) {
      for(const auto& e : v) {
        std::cout << e << " ";
      }
      std::cout << std::endl;
    }
  }
}

void init_neuralnet (NeuralNetType& nn, vector<size_t> sizes) {
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
  return;
}

//TODO:
//template<typename T> float calculate_accuracy(NeuralNetType& nn, ImageType& test) {
//  float accuracy = 0.0;
//  return accuracy;
//}

int estimate(const NeuralNetType& nn, const vector<vector<DataType>>& estimate_data) {
  int estimated_class = 0;
  size_t max_size = 0;
  for(const auto& layer : nn) max_size = max(max_size, layer.size());
  std::vector<float> nn_temp;
  nn_temp.reserve(max_size);
//  nn_temp_next.reserve(max_size);
  for(size_t i=0; i<nn.size(); ++i) {
    nn_temp.resize(nn[i].size());
    for(int j=0; j<nn_temp.size(); ++j) {
//      cout << nn[i][j].size() << endl;
//      cout << estimate_data.size() << endl;
      for(int k=0; k<estimate_data.size(); ++k) {
//        nn_temp[j] += nn[i][j][k]*estimate_data[k];
//        cout << k;
      }
//      view(nn_temp);
//      while(1){}
    }
  }
  return estimated_class;
}

void back_propagation(NeuralNetType& nn, ImageType& test) {
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

void learn(NeuralNetType& nn, ImageType& test) {
//  auto learning_process[](){
//    back_propagation(nn, test);
//  }
//  for_progress(100, learning_process);
  for(int i=0; i<100; ++i) {
    back_propagation(nn, test);
  }
  return;
}

void aoad_dataset (std::filesystem::path dataset_path, ImageType& x_train, std::vector<int>& y_train) {
  fstream file((dataset_path/"y.csv").c_str());
  string line;
  while (getline(file, line,'\n')) {
    istringstream oneline(line);
    string data;
    while (getline( oneline, data,',')) {
      if(data == " ") break;
      y_train.push_back(stoi(data));
    }
  }
  size_t data_col = 0, data_row = 0;
  {
    fstream file((dataset_path/"0.csv").c_str());
    while (getline(file, line,'\n')) {
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
//  x_train.resize(y_train.size(), std::vector<std::vector<int>>(data_col, std::vector<int>(data_row)));
  for(int i=0; i<y_train.size(); ++i) {
    fstream file((dataset_path/(to_string(i)+".csv")).c_str());
    size_t n = 0;
    while (getline(file, line,'\n')) {
      istringstream oneline(line);
      string data;
      size_t m = 0;
      while (getline(oneline, data,',')) {
        if(data==" ") break;
//        x_train[i][n][m] = stoi(data);
        m++;
      }
      n++;
    }
    file.close();
  }
  return;
}

int main() {
  vector<size_t> layers = {512, 256, 128, 64, 10};
//  neuralnet nn ( 10 );
//  neuralnet nn;
  neuralnet nn(layers);
  cout << nn.getLayers().size() << endl;
  view(nn.getLayers());
//  init_neuralnet(nn, layers);
  vector<int> y_train;
  ImageType x_train;
  std::filesystem::path train_path, test_path;
  {
    for(const auto& dir : DataPathVec) train_path /= dir;
    test_path = train_path / "test";
    train_path /= "train";
  }
  cout << train_path << endl;
  cout << test_path << endl;

//  dataset train;
//  train.load(train_path);
//  dataset test;
//  test.load(test_path);


//  train(10).draw();
//  cout << train.ans(10) << endl;

//  learn(nn, test);
//  view_network(nn);

//  vector<int> input(x_train.front().size()*x_train.front().front().size());
//  vector<vector<int>> input(x_train.front().size(), vector<int>(x_train.front().front().size()));
//  estimate(nn, input);

  return 0;
}
