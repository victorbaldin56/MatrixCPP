#pragma once

#include <vector>

#include "matrix.hh"

namespace matrix {

template <typename T>
class MatrixChain final {
 public:
  using size_type = typename Matrix<T>::size_type;

 public:
  MatrixChain() = default;

  template <typename... Args>
  auto emplace(Args&&... args) {
    matrices_.emplace_back(std::forward(args)...);
  }

 private:
  std::vector<size_type> sizes_;
  std::vector<Matrix<T>> matrices_;
};

}  // namespace matrix
