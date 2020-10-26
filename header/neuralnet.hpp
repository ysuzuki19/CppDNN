#ifndef NEURALNET_HPP_
#define NEURALNET_HPP_

#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <random>

#include <dataset.hpp>
#include <filesystem>
#include "../../YSL/include/CppProgressBar.hpp"
namespace fs = std::filesystem;

class NormalRandom {
	private:
		std::default_random_engine generator;
		std::normal_distribution<float> distribution;
	public:
		//NormalRandom () : distribution (0.0, 0.1) {}
		//NormalRandom () : distribution (0.0, 1.0) {}
		NormalRandom () : distribution (0.0, 10.0) {}
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
		//cout << "START ARGMAX" << endl;
		//Debug::view (src);
		//cout << "SIZE : " << src.size () << endl;
		int id = 0;
		float maximum = src[0];
		for (size_t i=1; i<src.size (); ++i) {
			//cout << "CHECK : " << src[i] << endl;
			if (maximum < src[i]) {
				id = i;
				maximum = src[i];
				//cout << id << " : " << maximum << endl;
			}
			//cout << "MAX : " << maximum << " at " << id << endl;
		}
		//cout << "ID : " << id << endl;
		return id;
	}
};

namespace Activation {
	inline float unit (float threshold, float input) {
		if (threshold <= input) return 1;
		return 0;
	}
	inline float sigmoid (float threshold, float input) {
		//return 1.0 / (1.0 + std::exp (input));
		return 1.0 / (1.0 + std::exp (-input + threshold));
	}
	inline float relu (float threshold, float input) {
		if (threshold <= input) return input - threshold;
		return 0;
	}
};

class NeuralNet {
	private:
		std::vector<std::size_t> layers_;

		float initial_neuron_threshold = 0.0;
		std::vector<std::vector<float>> neurons_; // each threashold
		//float initial_connect_weight = 1.0;
		float initial_connect_weight = 1.0;
		std::vector<std::vector<std::vector<float>>> connects_; // relations of neurons

		Dataset train_data_;
		Dataset test_data_;

		std::size_t input_size_ = 0;
		std::size_t output_size_ = 0;
		int batch_size_ = 100;

		float accuracy_ = 0.0;

		bool logging_ = true;

		void log_print (std::string str) {
			if (logging_) std::cout << str << std::endl;
		}

		NormalRandom nrand_;

		void initialization ();
		void back_propagation ();
		void simple_optim ();
		float calculate_accuracy (Dataset const& ds) const;

	public:
		NeuralNet () = default;
		NeuralNet (std::vector<std::size_t> layers, bool logging=true) : layers_{layers}, logging_{logging} {}

		void load (fs::path train_path, fs::path test_path, bool verbose);
		void fit ();

		std::vector<float> predict (data_type const& input) const;
		int predict_id (data_type const& input) const;
		float accuracy () const;

		// Accessor
		inline void setLayers (std::vector<std::size_t> layers) { layers_ = layers; this->initialization(); }
		inline std::vector<std::size_t> getLayers () const { return layers_; }
		inline std::vector<std::vector<float>> getNeurons () const { return neurons_; }
		inline std::vector<std::vector<vector<float>>> getConnect () const { return connects_; }
		inline data_type getTrain (std::size_t idx) const { return train_data_.get (idx); }
		inline std::size_t trainSize () const noexcept { return train_data_.size (); }
		inline std::size_t testSize () const noexcept { return test_data_.size (); }

		// Viewer
		void view_layers ();
		void view_neurons ();
		void view_connect ();
		void view_train_data (std::size_t idx);
		void view_test_data (std::size_t idx);
};

void NeuralNet::initialization () {
	neurons_.resize (layers_.size ());
	for (std::size_t layerIdx=0; layerIdx<layers_.size (); ++layerIdx) {
		neurons_[layerIdx].resize (layers_[layerIdx], initial_neuron_threshold);
	}
	log_print ("neurons was resized");

	connects_.resize (layers_.size () + 1);
	connects_[0].resize (input_size_, std::vector<float>(layers_[0], initial_connect_weight));
	for (std::size_t i=1; i<layers_.size (); ++i) {
		connects_[i].resize (layers_[i-1], std::vector<float>(layers_[i], initial_connect_weight));
	}
	connects_[layers_.size ()].resize (layers_.back (), std::vector<float>(output_size_, initial_connect_weight));
	log_print ("layers was resized");
}

// Data Loader
void NeuralNet::load (std::filesystem::path train_path, std::filesystem::path test_path, bool verbose=false) {
	log_print ("train data loaaing ...");
	train_data_.load (train_path);

	log_print ("test data loaaing ...");
	test_data_.load (test_path);

	input_size_ = train_data_.get (0).size ();
	log_print ("input size: " + std::to_string (input_size_));
	output_size_ = std::max (
			Support::max (train_data_.ans ()),
			Support::max (test_data_.ans ())
	) + 1; // ADD id=0

	log_print ("CLASSES : " + std::to_string (output_size_));

	this->initialization ();

	accuracy_ = this->calculate_accuracy (train_data_);
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
		for (std::size_t j=0; j<connects_[i].size (); ++j) {
			for (std::size_t k=0; k<connects_[i][j].size (); ++k) {
				float old_connect = connects_[i][j][k];
				connects_[i][j][k] += nrand_.generate ();
				float new_acc = this->calculate_accuracy (train_data_);
				if (accuracy_ < new_acc) {
					accuracy_ = new_acc;
				} else {
					connects_[i][j][k] = old_connect;
				}
			}
		}
	}
}
void NeuralNet::simple_optim () {
	auto old_connects = connects_;

	for (auto& layer_connects : connects_) {
		for (auto& connects : layer_connects) {
			for (auto& connect : connects) {
				connect += nrand_.generate ();
			}
		}
	}

	float new_acc = this->calculate_accuracy (train_data_);
	if (accuracy_ < new_acc) {
		accuracy_ = new_acc;
	} else {
		connects_ = old_connects;
	}
}
void NeuralNet::fit () {
	log_print ("train size : " + std::to_string (train_data_.size ()));
	log_print ("test size : " + std::to_string (test_data_.size ()));

	//batch_size_ = 10;//TODO: remove this line
	//batch_size_ = 50;//TODO: remove this line
	size_t cnt = 0;
	auto fitting_process = [&] (std::string& outputting) {
		this->back_propagation ();
		//this->simple_optim ();
		//outputting = to_string (cnt) + " : " + to_string (accuracy_);
		cnt ++;
	};

	for_progress (batch_size_, fitting_process);
}

std::vector<float> NeuralNet::predict (data_type const& input_data) const {
	std::vector<float> old_neurons;
	std::vector<float> new_neurons;

	new_neurons.resize (connects_[0][0].size (), 0.0);
	for (size_t j=0; j<connects_[0].size (); ++j) {
		for (size_t k=0; k<connects_[0][j].size (); ++k) {
			new_neurons[k] += (input_data.at (j) * connects_[0][j][k]);
		}
	}
	for (size_t k=0; k<new_neurons.size (); ++k) {
		new_neurons[k] = Activation::unit (neurons_[0][k], new_neurons[k]);
	}

	for (std::size_t i=1; i<connects_.size ()-1; ++i) {
		std::swap (old_neurons, new_neurons);
		new_neurons.resize (connects_[i][0].size (), 0.0);

		for (size_t j=0; j<connects_[i].size (); ++j) {
			for (size_t k=0; k<connects_[i][j].size (); ++k) {
				new_neurons[k] += (old_neurons[j] * connects_[i][j][k]);
			}
		}

		for (size_t k=0; k<connects_[i][0].size (); ++k) {
			new_neurons[k] = Activation::unit (neurons_[i][k], new_neurons[k]);
		}
	}

	std::vector<float> predictions (output_size_, 0.0);
	for (size_t j=0; j<connects_.back ().size (); ++j) {
		for (size_t k=0; k<connects_.back ()[j].size (); ++k) {
			predictions[k] += (new_neurons.at (j) * connects_.back ()[j][k]);
		}
	}
	//Debug::view (predictions);
	for (size_t k=0; k<connects_.back ()[0].size (); ++k) {
		predictions[k] = Activation::unit (neurons_.back ()[k], predictions[k]);
	}

	//Debug::view (predictions);
	//cout << "ANS : " << Support::argmax (predictions) << endl;
	//cout << endl;

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
		int pred_id = this->predict_id (dataset.get (i));
		int ans_id = dataset.ans (i);
		if (pred_id == ans_id) {
			ok++;
		}
	}
	return static_cast<float> (ok) / static_cast<float> (dataset.size ());
}
float NeuralNet::accuracy () const {
	return this->calculate_accuracy (test_data_);
}

#endif
