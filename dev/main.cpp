#include <data_type.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <random>

using namespace std;

//#include <filesystem_compatible_os.hpp> // NEED: if you use UBUNTU <= 18.04
#include <filesystem>
namespace fs = std::filesystem;
#include <debug.hpp>
#include <neuralnet.hpp>

using namespace std;
fs::path DatasetDir ("../dataset");

int main () {
  //vector<size_t> layers = {512, 256, 128, 64, 10};
  //vector<size_t> layers = {128, 64, 10};
  //vector<size_t> layers = {20, 10};
  vector<size_t> layers = {16};
  //vector<size_t> layers = {};
  NeuralNet nn (layers, false);
  //NeuralNet nn (layers);

  fs::path train_path, test_path;
  train_path = DatasetDir / "train";
  test_path = DatasetDir / "test";

  nn.load (train_path, test_path);
  cout << "INITIAL ACCURACY : " << nn.accuracy () << endl;
  //nn.load (train_path, test_path, true);

  //Debug::view ("layer size: ", nn.getLayers ().size ());
  //Debug::view ("neuron size: ", nn.getNeurons ().size ());
  //Debug::view ("connect size: ", nn.getConnect ().size ());
  //Debug::view ("layers : ", nn.getLayers ());
  //Debug::view ("neurons: ", nn.getNeurons ());
  //Debug::view ("connect: ", nn.getConnect ());
  //for (const auto& cons : nn.getConnect ()) {
    //cout << cons.size () << endl;
  ////}
  //nn.view_connect ();

  nn.fit ();

  //nn.view_connect ();
  cout << "FITTED ACCURACY  : " << nn.accuracy () << endl;
  //nn.calcAcc ();
  //Debug::view ("accuracy: ", nn.getAcc ());
  //Debug::view ("accuracy: ", nn.acc ());
  //cout << nn.acc () << endl;
  //auto at = nn.getTrain (0);
  //nn.predict ();
  //nn.view_train_data (0);
  //nn.view_neurons ();
  return 0;
}
