#ifndef DATATYPE_HPP
#define DATATYPE_HPP

#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

class data_type {
  private:
    std::size_t rows_ = 1;
    std::size_t cols_ = 1;
    std::vector<int> values_;
  public:
    data_type () = default;
    data_type (std::size_t rows, std::size_t cols) :
      rows_{rows},
      cols_{cols},
      values_ (rows*cols) {}

    int& operator () (std::size_t idx) {
      return values_.at (idx);
    }
    const int& operator () (std::size_t idx) const {
      return values_.at (idx);
    }
    int& operator () (std::size_t i, std::size_t j) {
      return values_.at (rows_*j + i);
    }
    const int& operator () (std::size_t i, std::size_t j) const {
      return values_.at (rows_*j + i);
    }

    std::size_t size () const noexcept {
      return rows_*cols_;
    }
    std::size_t rows () const noexcept {
      return rows_;
    }
    std::size_t cols () const noexcept {
      return cols_;
    }

    int at (std::size_t idx) const noexcept {
      return values_[idx];
    }

    inline void resize (std::size_t rows, std::size_t cols) {
      values_.resize (rows*cols);
      rows_ = rows;
      //cols_ = cols;
    }

    void load (std::string path);
    void draw () const;
    void draw_flatt () const;
};

void data_type::load (std::string path) {
  std::fstream file (path);
  std::size_t j = 0;
  std::string line;
  while (getline (file, line,'\n')) {
    std::istringstream oneline (line);
    std::size_t i = 0;
    std::string data;
    while (getline (oneline, data,',')) {
      if (data==" ") break;
      values_.at (rows_*j + i) = stoi (data);
      i++;
    }
    j++;
  }
  file.close ();
}

void data_type::draw () const {
  std::size_t data_number = 1;
  for (const auto& e : values_) {
    std::cout << std::setw (3) << e << " ";
    if (data_number % rows_ == 0) std::cout << std::endl;
    data_number++;
  }
}

void data_type::draw_flatt () const {
  for (const auto& e : values_) {
    std::cout << std::setw (3) << e << " ";
  }
  std::cout << std::endl;
}

#endif
