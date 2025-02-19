/**
 * Matrix implementation from scratch.
 */
#pragma once

#include <algorithm>
#include <cstddef>
#include <cmath>
#include <stdexcept>
#include <vector>

#include "numeric_traits.hh"

namespace matrix {

template <typename T>
class Matrix {
  std::vector<T> data_;
  std::size_t rows_;
  std::size_t cols_;

 public:
  using Iterator = typename std::vector<T>::iterator;
  using ConstIterator = typename std::vector<T>::const_iterator;
  using ReverseIterator = typename std::vector<T>::reverse_iterator;
  using ConstReverseIterator = typename std::vector<T>::const_reverse_iterator;

 public:
  /** Creates and fills matrix with given value */
  Matrix(std::size_t rows, std::size_t cols, T val = T())
      : data_(rows * cols, val), rows_(rows), cols_(cols) {}

  /** Creates matrix from given sequence */
  template <typename It>
  Matrix(std::size_t rows, std::size_t cols, It begin, It end)
      : rows_(rows), cols_(cols),
        data_(begin, std::min(end, begin + rows * cols)) {}

  template <typename It>
  Matrix(std::size_t cols, It begin, It end)
      : data_(begin, end),
        cols_(cols),
        rows_(std::ceil(static_cast<double>(data_.size()) / cols)) {}

 public:
  class Row {
    Iterator it_;

   public:
    Row(Iterator it) : it_(it) {}
    T& operator[](std::size_t pos) { return it_[pos]; }
    const T& operator[](std::size_t pos) const { return it_[pos]; }
  };

 public:
  Row operator[](std::size_t pos) { return Row(data_.begin() + pos * cols_); }

  auto begin() { return data_.begin(); }
  auto end() { return data_.end(); }
  auto cbegin() const { return data_.cbegin(); }
  auto cend() const { return data_.cend(); }

  auto rbegin() { return data_.rbegin(); }
  auto rend() { return data_.rend(); }
  auto crbegin() const { return data_.crbegin(); }
  auto crend() const { return data_.crend(); }

  auto rows() { return rows_; }
  auto cols() { return cols_; }

  /** Creates eye matrix */
  static Matrix eye(std::size_t n) {
    Matrix m(n, n);
    for (std::size_t i = 0; i < m.rows(); ++i) {
      // FIXME: не факт что скастуется
      m[i][i] = static_cast<T>(1);
    }
    return m;
  }

  bool swapRows(std::size_t a, std::size_t b) {
    if (a == b) {
      return false;
    }

    auto a_offset = a * cols_;
    auto b_offset = b * cols_;
    for (auto i = 0; i < cols_; ++i) {
      std::swap(data_[a_offset + i], data_[b_offset + i]);
    }
    return true;
  }

  void simplifyRows(std::size_t idx) {
    auto i_offset = idx * cols_;
    for (auto j = idx + 1; j < rows_; ++j) {
      auto shift_j = j * cols_;

      auto coef = data_[shift_j + idx] / data_[i_offset + idx];
      for (unsigned k = 0; k < cols_; ++k) {
        data_[shift_j + k] -= coef * data_[i_offset + k];
      }
    }
  }

  T det() const {
    // FIXME
    if (rows_ != cols_) {
      throw std::runtime_error("Not square matrix");
    }

    auto mcopy(*this);
    auto sign = 1;
    for (auto i = 0; i < cols_; ++i) {
      auto pivot = i;
      for (auto j = i + 1; j < rows_; ++j) {
        if (std::abs(mcopy[j][i]) > std::abs(mcopy[pivot][i])) {
          pivot = j;
        }
      }

      if (mcopy.swapRows(i, pivot)) {
        sign = -sign;
      }

      if (numeric_traits::isClose<double>(mcopy[i][i], 0)) {
        return 0;
      }
      mcopy.simplifyRows(i);
    }

    T det = static_cast<T>(sign);
    for (auto i = 0; i < rows_; ++i) {
      det *= mcopy[i][i];
    }
    return det;
  }
};

} // namespace matrix
