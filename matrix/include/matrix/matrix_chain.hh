#pragma once

#include <vector>

#include "matrix.hh"

namespace matrix {

template <typename T>
class MatrixChain final {
 public:
  MatrixChain() = default;

  template <typename... Args>
  auto emplace(Args&&... args) {
    matrices_.emplace_back(std::forward(args)...);
  }

  auto multiply() const {}

 private:
  std::vector<Matrix<T>> matrices_;
};

}  // namespace matrix
