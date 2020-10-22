#ifndef NEURALNET_HPP_
#define NEURALNET_HPP_

#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <dataset.hpp>
#include <random>

class NormalRandom {
	private:
		std::default_random_engine generator;
		std::normal_distribution<float> distribution;
	public:
		NormalRandom () : distribution (0.0, 0.0) {}
		NormalRandom  (float mean, float dist) : distribution (mean, dist) {}

		float generate () { return distribution (generator); }
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
		bool logging_ = true;
		template <typename T> inline T
		relu (T threshold, T input) {
			if (input - threshold <= 0) return 0;
			return input - threshold;
		}

		void
		setDataLayer () {
			if (connect_[0].size () != data_layer_) {
				connect_[0].resize (data_layer_);
			}
		}

		void
		init ();

		void
		back_propagation ();

		void
		calculate_accuracy ();

		void
		log_print (std::string str) { if (logging_) std::cout << str << std::endl; }
		NormalRandom nrand_;

	public:
		NeuralNet () = default;
		NeuralNet (std::size_t layer, bool logging=true) :
			layers_ (layer),
			neurons_ (layer),
			connect_ (layer),
			logging_{logging}
		{
			this->init ();
		}

		NeuralNet (std::vector<std::size_t> layers, bool logging=true) :
			layers_{layers},
			logging_{logging}
		{
			log_print ("constructing ... ");
			this->init ();
		}

		void
		load (std::filesystem::path train_path, std::filesystem::path test_path, bool verbose);

		void
		load_silent (std::filesystem::path train_path, std::filesystem::path test_path);

		void
		fit ();

		std::vector<float>
		predict (data_type const& input);

		int
		predict_id (data_type const& input);

		inline void
		setLayers (std::vector<std::size_t> layers) { layers_ = layers; init(); }

		inline std::vector<std::size_t>
		getLayers () { return layers_; }

		inline std::vector<std::vector<float>>
		getNeurons () { return neurons_; }

		inline std::vector<std::vector<float>>
		getConnect () { return connect_; }

		inline auto
		getTrain (std::size_t idx) { return train_data_.get (idx); }

		inline auto
		trainSize () const noexcept { return train_data_.size (); }

		inline auto
		testSize () const noexcept { return test_data_.size (); }

		int
		accuracy () noexcept;

		void
		view_layers ();
		void
		view_neurons ();
		void
		view_connect ();
		void
		view_train_data (std::size_t idx);
		void
		view_test_data (std::size_t idx);

};

void
NeuralNet::init () {
	neurons_.resize (layers_.size ());
	for (std::size_t layerIdx=0; layerIdx<layers_.size (); ++layerIdx) {
		neurons_[layerIdx].resize (layers_[layerIdx], 1.0);
	}
	log_print ("neurons was resized");
	temp_neurons_.resize (layers_.size ());
	for (std::size_t layerIdx=0; layerIdx<layers_.size (); ++layerIdx) {
		temp_neurons_[layerIdx].resize (layers_[layerIdx], 1.0);
	}
	log_print ("temp neurons was resized");
	connect_.resize (layers_.size ()+2);
	log_print ("connect size : ");
	log_print ("data layer : ");
	if (data_layer_) connect_[0].resize (data_layer_, 1.0);
	for (std::size_t layerIdx=0; layerIdx<layers_.size (); ++layerIdx) {
		connect_[layerIdx+1].resize (layers_[layerIdx], 1.0);
	}
	connect_[layers_.size ()+1].resize (classes_);
	log_print ("layers was resized");

	temp_connect_.resize (layers_.size ()+2);
	temp_connect_[0].resize (data_layer_, 1.0);
	for (std::size_t layerIdx=0; layerIdx<layers_.size (); ++layerIdx) {
		temp_connect_[layerIdx+1].resize (layers_[layerIdx], 1.0);
	}
	temp_connect_[layers_.size ()+1].resize (classes_);
	log_print ("temp connect was resized");

	input_buffer_.resize (layers_.size ()+2); // add data_layer and ans_layer
	log_print ("input_buffer_ size: " + std::to_string (input_buffer_[0].size ()));
	//input_buffer_[0].resize (data_layer_, 1.0);
	this->setDataLayer ();
	log_print ("input_buffer_ size: " + std::to_string (input_buffer_[0].size ()));
	for (std::size_t layerIdx=0; layerIdx<layers_.size (); ++layerIdx) {
		input_buffer_[layerIdx+1].resize (layers_[layerIdx], 1.0);
	}
	input_buffer_[layers_.size ()+1].resize (classes_);
	return;
}

// Data Loader
void 
NeuralNet::load (std::filesystem::path train_path, std::filesystem::path test_path, bool verbose=false) {
	if (verbose) std::clog << "train data loaaing ..." << std::endl;
	train_data_.load (train_path);
	if (verbose) std::clog << "test data loaaing ..." << std::endl;
	train_data_.load (train_path);
	test_data_.load (test_path);
	data_layer_ = train_data_.cols () * train_data_.rows ();
	//data_layer_.resize (train_data_.cols ()*train_data_.rows ());
	for (std::size_t i=0; i<train_data_.size (); ++i) {
		if (classes_ < train_data_.ans (i)) {
			classes_ = train_data_.ans (i);
		}
	}
	if (verbose) std::clog << "neuralnet initializing ..." << std::endl;
	this->setDataLayer ();
	return;
}

// Data Viewers
void 
NeuralNet::view_layers () {
	for (const auto& layer_size : layers_) {
		std::cout << layer_size << " ";
	}
		std::cout << std::endl;
	return;
}
void 
NeuralNet::view_neurons () {
	for (const auto& layer_neuron : neurons_) {
		for (const auto& neuron : layer_neuron) {
			std::cout << neuron << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	return;
}
void 
NeuralNet::view_connect () {
	for (const auto& layer_connect : connect_) {
		for (const auto& connect : layer_connect) {
			std::cout << connect << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	return;
}
void 
NeuralNet::view_train_data (std::size_t idx) {
	train_data_.get (idx).draw ();
}
void 
NeuralNet::view_test_data (std::size_t idx) {
	train_data_.get (idx).draw ();
}
void 
NeuralNet::back_propagation () {
	cout << "back propagate" << endl;
	cout << temp_connect_.size () << endl;
	for (size_t i=1; i<layers_.size (); ++i) {
		cout << temp_connect_[i].size () << endl;
		for (size_t j=0; j<layers_[i]; ++j) {
			cout << temp_connect_[i][j] << " ";
			temp_connect_[i][j] = connect_[i][j] + nrand_.generate ();
		}
		cout << endl;
	}
}
void 
NeuralNet::fit () {
//	TODO: use following part
//	auto fitting_process[] () {
//		back_propagation (nn, test);
//	}
//	for_progress (batch_size_, fitting_process);
	batch_size_ = 1;//TODO: remove this line
	for (int i=0; i<batch_size_; ++i) {
		back_propagation ();
	}
	log_print ("train size : " + std::to_string (train_data_.size ()));
	log_print ("test size : " + std::to_string (test_data_.size ()));
	//predict (train_data_.get (0));
	return;
}

std::vector<float>
NeuralNet::predict (data_type const& input_data) {
	std::vector<float> predictions; 

	return predictions;
}

int
NeuralNet::predict_id (data_type const& input_data) {
	std::vector<float> predictions = predict (input_data);
	size_t id = 0;
	float maximum = predictions[0];
	for (size_t i=1; i<predictions.size (); ++i) {
		if (maximum < predictions[i]) {
			id = i;
			maximum = predictions[i];
		}
	}

	return id;
}

void 
NeuralNet::calculate_accuracy () {
	std::size_t ok = 0;
	for (std::size_t i=0; i<train_data_.size (); ++i) {
		if (predict_id (train_data_.get (i)) == train_data_.ans (i)) {
			ok++;
		}
	}
	accuracy_ = static_cast<float> (ok) / static_cast<float> (test_data_.size ());
}

int 
NeuralNet::accuracy () noexcept {
	this->calculate_accuracy ();
	return accuracy_;
}

#endif
