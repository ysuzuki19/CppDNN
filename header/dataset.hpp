#ifndef DATASET_HPP
#define DATASET_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <filesystem_compatible_os.hpp>
#include "data_type.hpp"

class dataset {
  private:
    std::size_t size_ = 0;
    std::vector<data_type> imgs_;
    std::vector<int> ans_;
    std::size_t unique_rows_ = 0;
    std::size_t unique_cols_ = 0;
    inline void resize(std::size_t size) {
      size_ = size;
      imgs_.resize(size_);
    }

  public:
    dataset() : imgs_(0), unique_rows_{0}, unique_cols_{0} {}
    dataset(std::size_t size) : imgs_(size), unique_rows_{0}, unique_cols_{0} {}
//    dataset(std::size_t size, std::size_t unique_rows, std::size_t unique_cols) : imgs_(size, data_type(unique_rows, unique_cols)), unique_rows_{unique_rows}, unique_cols_{unique_cols} {}

    auto size() const noexcept { return imgs_.size(); }
    auto& operator()(std::size_t i) { return imgs_.at(i); }
    const auto& operator()(std::size_t i) const { return imgs_.at(i); }
    auto rows() const noexcept { return unique_rows_; }
    auto cols() const noexcept { return unique_cols_; }
    inline auto ans () { return ans_; }
    inline int ans (size_t idx) { return ans_[idx]; }
    void load (fs::path dataset_path) ;
};

void dataset::load (fs::path dataset_path) {
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
  imgs_.resize(ans_.size());
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
    if(unique_cols_ != img_col || unique_rows_ != img_row) {
      unique_cols_ = img_col;
      unique_rows_ = img_row;
      for(auto& e : imgs_) e.resize(unique_rows_, unique_cols_);
    }
  }
  for(size_t i=0; i<ans_.size(); ++i) {
    imgs_[i].load(dataset_path/(std::to_string(i)+".csv"));
  }
  return;
}

#endif
