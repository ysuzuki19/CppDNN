#ifndef DATATYPE_HPP
#define DATATYPE_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

class data_type {
  private:
    std::size_t rows_;
    std::size_t size_;
    std::vector<int> data_;
  public:
    data_type() = default;
    data_type(std::size_t rows, std::size_t cols) : rows_{rows} {data_.resize(rows*cols);}

    auto& operator()(std::size_t i, std::size_t j) { return data_.at(rows_*j + i); }
    const auto& operator()(std::size_t i, std::size_t j) const { return data_.at(rows_*j + i); }

    auto size() const noexcept { return data_.size(); }
    auto rows() const noexcept { return rows_; }
    auto cols() const noexcept { return this->size()/rows_; }

    inline void resize(std::size_t rows, std::size_t cols) {
      data_.resize(rows*cols);
      rows_ = rows;
    }

    void load(std::string path);
    void draw();
    void draw_flatt();
};

void data_type::load(std::string path) {
  std::fstream file(path);
  std::size_t j = 0;
  std::string line;
  while (getline(file, line,'\n'))  {
    std::istringstream oneline(line);
    std::size_t i = 0;
    std::string data;
    while (getline(oneline, data,',')) {
      if(data==" ") break;
      data_.at(rows_*j + i) = stoi(data);
      i++;
    }
    j++;
  }
  file.close();
}

void data_type::draw() {
  std::size_t data_number = 1;
  for(const auto& e : data_) {
    std::cout << std::setw(3) << e << " ";
    if(data_number % rows_ == 0) std::cout << std::endl;
    data_number++;
  }
}

void data_type::draw_flatt() {
  for(const auto& e : data_) {
    std::cout << std::setw(3) << e << " ";
  }
  std::cout << std::endl;
}

#endif
