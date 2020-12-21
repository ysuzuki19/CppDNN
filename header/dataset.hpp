#ifndef DATASET_HPP
#define DATASET_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <filesystem_compatible_os.hpp>
#include "data_type.hpp"

class Dataset {
  private:
    std::size_t size_ = 0;
    std::vector<data_type> dataset_;
    std::vector<int> ans_;
    std::size_t each_rows_ = 0;
    std::size_t each_cols_ = 0;
    inline void resize(std::size_t size) {
      size_ = size;
      dataset_.resize(size_);
    }

  public:
    Dataset()
      : dataset_(0), each_rows_{0}, each_cols_{0} {}
    Dataset(std::size_t size)
      : dataset_(size), each_rows_{0}, each_cols_{0} {}
//    Dataset(std::size_t size, std::size_t each_rows, std::size_t each_cols) : dataset_(size, data_type(each_rows, each_cols)), each_rows_{each_rows}, each_cols_{each_cols} {}

    std::size_t size() const noexcept { return dataset_.size(); }
    data_type& operator()(std::size_t i) { return dataset_.at(i); }
    const data_type& operator()(std::size_t i) const { return dataset_.at(i); }
    std::size_t rows() const noexcept { return each_rows_; }
    std::size_t cols() const noexcept { return each_cols_; }
    inline std::vector<int> ans () const { return ans_; }
    inline int ans (size_t idx) const { return ans_[idx]; }
    void load (fs::path dataset_path);
    data_type const& get(std::size_t idx) const { return dataset_[idx]; }
};

void Dataset::load (fs::path dataset_path) {
  std::fstream file((dataset_path/"y.csv").c_str());
  std::string line;
  while (getline(file, line,'\n')) {
    std::istringstream oneline(line);
    std::string data;
    while (getline( oneline, data,',')) {
      if(data == " ") break;
      ans_.push_back(stoi(data));
    }
  }
  size_t img_col = 0, img_row = 0;
  dataset_.resize(ans_.size());
  {
    std::fstream file((dataset_path/"0.csv").c_str());
    while (getline(file, line,'\n')) {
      std::istringstream oneline(line);
      std::string data;
      img_row = 0;
      while (getline(oneline, data,',')) {
        if(data==" ") break;
        img_row++;
      }
      img_col++;
    }
    file.close();
    if(each_cols_ != img_col || each_rows_ != img_row) {
      each_cols_ = img_col;
      each_rows_ = img_row;
      for(auto& e : dataset_) e.resize(each_rows_, each_cols_);
    }
  }
  for(size_t i=0; i<ans_.size(); ++i) {
    dataset_[i].load(dataset_path/(std::to_string(i)+".csv"));
  }
  return;
}

#endif
