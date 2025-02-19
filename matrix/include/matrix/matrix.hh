/**
 * Matrix implementation from scratch.
 */
#pragma once

#include <algorithm>
#include <cstddef>
#include <cmath>
#include <stdexcept>
#include <type_traits>
#include <vector>

#include "numeric_traits.hh"

namespace matrix {

template <
    typename T,
    typename Enable = typename std::enable_if<
        std::is_convertible_v<int, T> &&
        std::is_same<
            decltype(std::declval<T>() + std::declval<T>()), T>::value &&
        std::is_same<
            decltype(std::declval<T>() - std::declval<T>()), T>::value &&
        std::is_same<
            decltype(std::declval<T>() * std::declval<T>()), T>::value &&
        std::is_same<
            decltype(std::declval<T>() / std::declval<T>()), T>::value>::type>
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
  template <
      typename It,
      typename E = typename std::enable_if<
          std::is_base_of<
              std::input_iterator_tag,
              typename
                  std::iterator_traits<It>::iterator_category>::value>::type>
  Matrix(std::size_t rows, std::size_t cols, It begin, It end)
      : rows_(rows), cols_(cols),
        data_(begin, std::min(end, begin + rows * cols)) {}

  template <
      typename It,
      typename E = typename std::enable_if<
          std::is_base_of<
              std::input_iterator_tag,
              typename
                  std::iterator_traits<It>::iterator_category>::value>::type>
  Matrix(std::size_t cols, It begin, It end)
      : data_(begin, end),
        cols_(cols),
        rows_(std::ceil(static_cast<double>(data_.size()) / cols)) {}

 public:
  Iterator operator[](std::size_t pos) noexcept {
    return data_.begin() + pos * cols_;
  }

  ConstIterator operator[](std::size_t pos) const noexcept {
    return data_.cbegin() + pos * cols_;
  }

  auto begin() noexcept { return data_.begin(); }
  auto end() noexcept { return data_.end(); }
  auto cbegin() const noexcept { return data_.cbegin(); }
  auto cend() const noexcept { return data_.cend(); }

  auto rbegin() noexcept { return data_.rbegin(); }
  auto rend() noexcept { return data_.rend(); }
  auto crbegin() const noexcept { return data_.crbegin(); }
  auto crend() const noexcept { return data_.crend(); }

  auto rows() const noexcept { return rows_; }
  auto cols() const noexcept { return cols_; }

  /** Creates eye matrix */
  static Matrix eye(std::size_t n) {
    Matrix m(n, n);
    for (std::size_t i = 0; i < m.rows(); ++i) {
      m[i][i] = static_cast<T>(1);
    }
    return m;
  }

  bool swapRows(std::size_t a, std::size_t b) noexcept {
    if (a == b) {
      return false;
    }
    auto ait = operator[](a);
    std::swap_ranges(ait, ait + cols_, operator[](b));
    return true;
  }

  void simplifyRows(std::size_t idx) {
    auto i_offset = idx * cols_;
    for (auto j = idx + 1; j < rows_; ++j) {
      auto shift_j = j * cols_;

      auto jit = data_.begin() + shift_j;
      auto coef = data_[shift_j + idx] / data_[i_offset + idx];
      std::transform(
          data_.begin() + i_offset, data_.begin() + i_offset + cols_,
          jit, jit,
          [coef](const auto& a, const auto& b) { return b - coef * a; });
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
