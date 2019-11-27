#ifndef NEURALNET_HPP_
#define NEURALNET_HPP_

template <typename T>
inline T relu (T threshold, T input) {
  if(input - threshold <= 0) return 0;
  else return input - threshold;
}

class neuralnet {
  private:
    std::size_t size_ = 0;
    std::vector<std::size_t> sizes_;
    std::vector<std::vector<float>> network_;

  public:
//    dataset() : imgs_(0), unique_rows_{0}, unique_cols_{0} {}
//    dataset(std::size_t size) : imgs_(size), unique_rows_{0}, unique_cols_{0} {}
//
//    auto size() const noexcept { return imgs_.size(); }
//    auto& operator()(std::size_t i) { return imgs_.at(i); }
//    const auto& operator()(std::size_t i) const { return imgs_.at(i); }
//    auto rows() const noexcept { return unique_rows_; }
//    auto cols() const noexcept { return unique_cols_; }
//    inline int ans (size_t idx) { return ans_[idx]; }
//    void load (std::filesystem::path dataset_path) ;
};

#endif
