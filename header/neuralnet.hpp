#ifndef NEURALNET_HPP_
#define NEURALNET_HPP_

#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <random>

#include <dataset.hpp>
#include <filesystem>
namespace fs = std::filesystem;

class NormalRandom {
	private:
		std::default_random_engine generator;
		std::normal_distribution<float> distribution;
	public:
		//NormalRandom () : distribution (0.0, 0.1) {}
		NormalRandom () : distribution (0.0, 1.0) {}
		NormalRandom  (float mean, float dist) : distribution (mean, dist) {}

		float generate () { return distribution (generator); }
};

namespace Support {
	int max (std::vector<int> const& src) {
		int maximum = src[0];
		for (std::size_t i=1; i<src.size (); ++i) {
			maximum = std::max (maximum, src[i]);
		}
		return maximum;
	}

	int argmax (std::vector<float> const& src) {
		int id = 0;
		float maximum = src[0];
		for (size_t i=1; i<src.size (); ++i) {
			if (maximum < src[i]) {
				id = i;
				maximum = src[i];
			}
		}
		return id;
	}

	inline float relu (float threshold, float input) {
		if (input - threshold <= 0) return 0;
		return input - threshold;
	}
};

class NeuralNet {
	private:
		std::vector<std::size_t> layers_;

		std::vector<std::vector<float>> neurons_; // each threashold
		std::vector<std::vector<std::vector<float>>> connects_; // relations of neurons

		Dataset train_data_;
		Dataset test_data_;

		std::size_t input_size_ = 0;
		std::size_t output_size_ = 0;
		int classes_ = 0;

		int batch_size_ = 100;

		float accuracy_ = 0.0;

		bool logging_ = true;

		void log_print (std::string str) {
			if (logging_) std::cout << str << std::endl;
		}

		NormalRandom nrand_;

		void initialization ();

		void back_propagation ();

		float calculate_accuracy (Dataset const& ds) const;

	public:
		NeuralNet () = default;
		NeuralNet (std::vector<std::size_t> layers, bool logging=true) : layers_{layers}, logging_{logging} {}

		void load (fs::path train_path, fs::path test_path, bool verbose);

		void fit ();

		std::vector<float> predict (data_type const& input) const;

		int predict_id (data_type const& input) const;

		inline void setLayers (std::vector<std::size_t> layers) { layers_ = layers; this->initialization(); }
		inline std::vector<std::size_t> getLayers () const { return layers_; }
		inline std::vector<std::vector<float>> getNeurons () const { return neurons_; }
		inline std::vector<std::vector<vector<float>>> getConnect () const { return connects_; }
		inline data_type getTrain (std::size_t idx) const { return train_data_.get (idx); }
		inline std::size_t trainSize () const noexcept { return train_data_.size (); }
		inline std::size_t testSize () const noexcept { return test_data_.size (); }

		float accuracy () const;

		void view_layers ();
		void view_neurons ();
		void view_connect ();
		void view_train_data (std::size_t idx);
		void view_test_data (std::size_t idx);
};

void NeuralNet::initialization () {
	neurons_.resize (layers_.size ());
	for (std::size_t layerIdx=0; layerIdx<layers_.size (); ++layerIdx) {
		neurons_[layerIdx].resize (layers_[layerIdx], 1.0);
	}
	log_print ("neurons was resized");

	float initial_threashold = 0.0;

	connects_.resize (layers_.size () + 1);
	connects_[0].resize (input_size_, std::vector<float>(layers_[0], initial_threashold));
	for (std::size_t i=1; i<layers_.size (); ++i) {
		connects_[i].resize (layers_[i-1], std::vector<float>(layers_[i], initial_threashold));
	}
	connects_[layers_.size ()].resize (layers_.back (), std::vector<float>(output_size_, initial_threashold));
	log_print ("layers was resized");
}

// Data Loader
void NeuralNet::load (std::filesystem::path train_path, std::filesystem::path test_path, bool verbose=false) {
	if (verbose) std::clog << "train data loaaing ..." << std::endl;
	train_data_.load (train_path);

	if (verbose) std::clog << "test data loaaing ..." << std::endl;
	test_data_.load (test_path);

	input_size_ = train_data_.get (0).size ();
	output_size_ = std::max (
			Support::max (train_data_.ans ()),
			Support::max (test_data_.ans ())
	) + 1; // ADD id=0

	if (verbose) std::cout << "CLASSES : " << output_size_ << std::endl;

	this->initialization ();
}

// Data Viewers
void NeuralNet::view_layers () {
	for (const auto& layer_size : layers_) {
		std::cout << layer_size << " ";
	}
		std::cout << std::endl;
}
void NeuralNet::view_neurons () {
	for (const auto& layer_neuron : neurons_) {
		for (const auto& neuron : layer_neuron) {
			std::cout << neuron << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
void NeuralNet::view_connect () {
	std::cout << "=============== CONNECTS ========================================" << endl;
	for (const auto& layers_connects : connects_) {
		for (const auto& connects : layers_connects) {
			for (const auto& connect : connects) {
				std::cout << connect << " ";
			}
			std::cout << std::endl;
		}
	}
	std::cout << "=================================================================" << endl;
}
void NeuralNet::view_train_data (std::size_t idx) {
	train_data_.get (idx).draw ();
}
void NeuralNet::view_test_data (std::size_t idx) {
	train_data_.get (idx).draw ();
}
void NeuralNet::back_propagation () {
	for (std::size_t i=connects_.size ()-1; i<connects_.size (); --i) {
		for (std::size_t j=connects_[i].size ()-1; j<connects_[i].size (); --j) {
			for (std::size_t k=connects_[i][j].size ()-1; k<connects_[i][j].size (); --k) {
				float old_connect = connects_[i][j][k];
				connects_[i][j][k] += nrand_.generate ();
				float new_acc = this->calculate_accuracy (train_data_);
				//cout << new_acc << endl;
				if (accuracy_ > new_acc) {
					//cout << "DONT UPDATE" << endl;
					connects_[i][j][k] = old_connect;
				} else {
					//cout << "UPDATE" << endl;
					accuracy_ = new_acc;
				}
			}
		}
	}
}
void NeuralNet::fit () {
//TODO: use following part
//	auto fitting_process[] () {
//		this->back_propagation (nn, test);
//	}
//	for_progress (batch_size_, fitting_process);
	batch_size_ = 1;//TODO: remove this line
	for (int i=0; i<batch_size_; ++i) {
		this->back_propagation ();
	}
	log_print ("train size : " + std::to_string (train_data_.size ()));
	log_print ("test size : " + std::to_string (test_data_.size ()));
}

std::vector<float> NeuralNet::predict (data_type const& input_data) const {
	std::vector<float> odd_neurons;
	std::vector<float> even_neurons;

	// process input layer
	odd_neurons.resize (connects_[0][0].size (), 0.0);
	for (size_t j=0; j<connects_[0].size (); ++j) {
		for (size_t k=0; k<connects_[0][j].size (); ++k) {
			odd_neurons[k] += Support::relu (neurons_[0][k], input_data.at (j) * connects_[0][j][k]);
		}
	}

	bool is_odd = false;

	for (std::size_t i=1; i<connects_.size ()-1; ++i) {
		if (is_odd) {
			odd_neurons.resize (connects_[i+1][0].size (), 0.0);
		} else {
			even_neurons.resize (connects_[i+1][0].size (), 0.0);
		}
		for (size_t j=0; j<connects_[i].size (); ++j) {
			for (size_t k=0; k<connects_[i][j].size (); ++k) {
				if (is_odd) {
					odd_neurons[k] += Support::relu (neurons_[i][k], even_neurons[j] * connects_[i][j][k]);
				} else {
					even_neurons[k] += (odd_neurons[j] * connects_[i][j][k]);
				}
			}
		}
		is_odd = (!is_odd);
	}

	// process output layer
	std::vector<float> predictions (output_size_, 0.0);
	//cout << endl;
	//cout << "OUTPUT SIZE : " << output_size_ << endl;
	//cout << "CONNECTS SIZE : " << connects_.back ()[0].size () << endl;
	for (size_t j=0; j<connects_.back ().size (); ++j) {
		for (size_t k=0; k<connects_.back ()[j].size (); ++k) {
			if (is_odd) {
				predictions[k] += Support::relu (neurons_.back ()[k], even_neurons.at (j) * connects_.back ()[j][k]);
			} else {
				predictions[k] += Support::relu (neurons_.back ()[k], odd_neurons.at (j) * connects_.back ()[j][k]);
			}
		}
	}

	//debug::view (predictions);

	return predictions;
}

int NeuralNet::predict_id (data_type const& input_data) const {
	std::vector<float> predictions = this->predict (input_data);
	int id = Support::argmax (predictions);
	return id;
}
float NeuralNet::calculate_accuracy (Dataset const& dataset) const {
	std::size_t ok = 0;
	for (std::size_t i=0; i<dataset.size (); ++i) {
		if (this->predict_id (dataset.get (i)) == dataset.ans (i)) {
			ok++;
		}
	}
	return static_cast<float> (ok) / static_cast<float> (dataset.size ());
}
float NeuralNet::accuracy () const {
	return this->calculate_accuracy (test_data_);
}

#endif
