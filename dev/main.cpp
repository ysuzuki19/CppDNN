#include <data_type.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <random>

using namespace std;

#include <filesystem_compatible_os.hpp>
#include <debug.hpp>
#include <neuralnet.hpp>

using namespace std;
fs::path DatasetPath ("../dataset");

int main(){
	//vector<size_t> layers = {512, 256, 128, 64, 10};
	//vector<size_t> layers = {128, 64, 10};
	vector<size_t> layers = {16, 4};
	NeuralNet nn(layers);
	debug::view("layer size: ", nn.getLayers().size());
	debug::view("layers : ", nn.getLayers());
	debug::view("neurons: ", nn.getNeurons());
	debug::view("connect: ", nn.getConnect());

	fs::path train_path, test_path;
	train_path = DatasetPath / "train";
	test_path = DatasetPath / "test";

	//nn.load(train_path, test_path);
	nn.load(train_path, test_path, true);
	cout << "start fit " << endl;
	nn.fit();
	//nn.calcAcc();
	//debug::view("accuracy: ", nn.getAcc());
	debug::view("accuracy: ", nn.acc());
	//cout << nn.acc() << endl;
	//auto at = nn.getTrain(0);
	//nn.predict();
//	nn.view_train_data(0);
//	nn.view_connect();
//	nn.view_neurons();
	return 0;
}
