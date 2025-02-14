/**
 * Matrix implementation from scratch.
 */
#pragma once

#include <cstddef>

namespace matrix {

template <typename T>
class Matrix {
  // internal state

 public:
  /** Creates and fills matrix with given value */
  Matrix(std::size_t rows, std::size_t cols, T val = T());

  /** Creates matrix from given sequence */
  template <typename It>
  Matrix(std::size_t rows, std::size_t cols, It begin, It end);

  /** Creates eye matrix */
  static Matrix eye(std::size_t n);

  Matrix& negate() &;
  Matrix& transpose() &;
};

} // namespace matrix
