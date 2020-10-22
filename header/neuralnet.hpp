#ifndef NEURALNET_HPP_
#define NEURALNET_HPP_

#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <dataset.hpp>

class NormalRandom {
	private:
		std::default_random_engine generator;
		std::normal_distribution<float> distribution{0.0, 0.1};
	public:
		NormalRandom () = default;
		// TODO: distributionをカスタム設定 at コンストラクタ
		//NormalRandom (float mean, float dist) = default;
		float gen(){ return distribution(generator); }
};

class NeuralNet {
	private:
		std::size_t data_layer_ = 0;
		std::vector<std::size_t> layers_;
		std::vector<std::vector<float>> neurons_;
		std::vector<std::vector<float>> connect_;
		std::vector<std::vector<float>> temp_neurons_;
		std::vector<std::vector<float>> temp_connect_;
		std::vector<std::vector<float>> input_buffer_;
		dataset train_data_;
		dataset test_data_;
		float accuracy_ = 0.0;
		int batch_size_ = 100;
		int classes_ = 0;
		bool logging = true;
		template <typename T> inline T relu(T threshold, T input);
		void setDataLayer() {
			if (connect_[0].size() != data_layer_) {
				connect_[0].resize(data_layer_);
			}
		}
		void init();
		void back_propagation();
		void calcAcc(); // calculate accuracy from test data
		void log_print(std::string str) { if(logging) std::cout << str << endl; }
		NormalRandom nrand_;
	public:
		NeuralNet() = default;
		NeuralNet(std::size_t layer)
			: layers_(layer),
				neurons_(layer),
				connect_(layer)
				{
					this->init();
				}
		NeuralNet(std::vector<std::size_t> layers)
			: layers_{layers} {
				cout << "constructing ... " << endl;
				this->init();
			}

		void load (std::filesystem::path train_path, std::filesystem::path test_path, bool verbose);
		void load_silent (std::filesystem::path train_path, std::filesystem::path test_path);
		void fit();
		int predict(data_type input);
		inline void setLayers (std::vector<std::size_t> layers){ layers_ = layers; init(); }
		inline std::vector<std::size_t> getLayers (){ return layers_; }
		inline std::vector<std::vector<float>> getNeurons (){ return neurons_; }
		inline std::vector<std::vector<float>> getConnect (){ return connect_; }
		inline auto getTrain(std::size_t idx) { return train_data_.get(idx); }
		inline auto trainSize() const noexcept { return train_data_.size(); }
		inline auto testSize() const noexcept { return test_data_.size(); }
		int acc() noexcept;
		void view_layers ();
		void view_neurons ();
		void view_connect ();
		void view_train_data (std::size_t idx);
		void view_test_data (std::size_t idx);
};

template <typename T> inline T NeuralNet::relu (T threshold, T input){
	if(input - threshold <= 0) return 0;
	else return input - threshold;
}

void NeuralNet::init(){
	neurons_.resize(layers_.size());
	for(std::size_t layerIdx=0; layerIdx<layers_.size(); ++layerIdx){
		neurons_[layerIdx].resize(layers_[layerIdx], 1.0);
	}
	cout << "neurons was resized" << endl;
	temp_neurons_.resize(layers_.size());
	for(std::size_t layerIdx=0; layerIdx<layers_.size(); ++layerIdx){
		temp_neurons_[layerIdx].resize(layers_[layerIdx], 1.0);
	}
	cout << "temp neurons was resized" << endl;
	connect_.resize(layers_.size()+2);
	cout << "connect size : " << connect_.size() << endl;
	cout << "data layer : " << data_layer_ << endl;
	if (data_layer_) connect_[0].resize(data_layer_, 1.0);
	for(std::size_t layerIdx=0; layerIdx<layers_.size(); ++layerIdx){
		connect_[layerIdx+1].resize(layers_[layerIdx], 1.0);
	}
	connect_[layers_.size()+1].resize(classes_);
	cout << "layers was resized" << endl;

	temp_connect_.resize(layers_.size()+2);
	temp_connect_[0].resize(data_layer_, 1.0);
	for(std::size_t layerIdx=0; layerIdx<layers_.size(); ++layerIdx){
		temp_connect_[layerIdx+1].resize(layers_[layerIdx], 1.0);
	}
	temp_connect_[layers_.size()+1].resize(classes_);
	cout << "temp connect was resized" << endl;

	input_buffer_.resize(layers_.size()+2); // add data_layer and ans_layer
	cout << "input_buffer_ size: " << input_buffer_[0].size() << endl;
	//input_buffer_[0].resize(data_layer_, 1.0);
	this->setDataLayer();
	//cout << "input_buffer_ size: " << input_buffer_[0].size() << endl;
	for(std::size_t layerIdx=0; layerIdx<layers_.size(); ++layerIdx){
		input_buffer_[layerIdx+1].resize(layers_[layerIdx], 1.0);
	}
	input_buffer_[layers_.size()+1].resize(classes_);
	return;
}

// Data Loader
void NeuralNet::load(std::filesystem::path train_path, std::filesystem::path test_path, bool verbose=false){
	if (verbose) std::clog << "train data loaaing ..." << std::endl;
	train_data_.load(train_path);
	if (verbose) std::clog << "test data loaaing ..." << std::endl;
	train_data_.load(train_path);
	test_data_.load(test_path);
	data_layer_ = train_data_.cols() * train_data_.rows();
	//data_layer_.resize(train_data_.cols()*train_data_.rows());
	for(std::size_t i=0; i<train_data_.size(); ++i){
		if(classes_ < train_data_.ans(i)){
			classes_ = train_data_.ans(i);
		}
	}
	if (verbose) std::clog << "neuralnet initializing ..." << std::endl;
	this->setDataLayer();
	return;
}

// Data Viewers
void NeuralNet::view_layers (){
	for(const auto& layer_size : layers_){
		std::cout << layer_size << " ";
	}
		std::cout << std::endl;
	return;
}
void NeuralNet::view_neurons (){
	for(const auto& layer_neuron : neurons_){
		for(const auto& neuron : layer_neuron){
			std::cout << neuron << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	return;
}
void NeuralNet::view_connect (){
	for(const auto& layer_connect : connect_){
		for(const auto& connect : layer_connect){
			std::cout << connect << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	return;
}
void NeuralNet::view_train_data (std::size_t idx){
		train_data_.get(idx).draw();
}
void NeuralNet::view_test_data (std::size_t idx){
		train_data_.get(idx).draw();
}
void NeuralNet::back_propagation(){
	for(size_t i=0; i<layers_.size(); ++i){
		for(size_t j=0; j<layers_[i]; ++j){
			temp_connect_[i][j] = connect_[i][j] + nrand_.gen();
		}
	}
}
void NeuralNet::fit(){
//	TODO: use following part
//	auto fitting_process[](){
//		back_propagation(nn, test);
//	}
//	for_progress(batch_size_, fitting_process);
	for(int i=0; i<batch_size_; ++i){
		back_propagation();
	}
	log_print("train size : " + std::to_string(train_data_.size()));
	log_print("test size : " + std::to_string(test_data_.size()));
	predict(train_data_.get(0));
	return;
}
int NeuralNet::predict(data_type input_data){
	//log_print(std::to_string(input_data.size()));
	//log_print(std::to_string(data_layer_.size()));
	int estimated_class = 0;
	return estimated_class;
}
void NeuralNet::calcAcc(){
	std::size_t ok = 0;
	for(std::size_t i=0; i<train_data_.size(); ++i){
		if(predict(train_data_.get(i)) == train_data_.ans(i)){
			ok++;
		}
	}
	accuracy_ = ok / test_data_.size();
	return;
}

int NeuralNet::acc() noexcept {
	this->calcAcc();
	return accuracy_;
}

#endif
