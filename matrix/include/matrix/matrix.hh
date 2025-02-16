/**
 * Matrix implementation from scratch.
 */
#pragma once

#include <iterator>
#include <initializer_list>

#include "detail/matrix_buffer.hh"

namespace matrix {

template <typename T>
class Matrix : private detail::VectorBuffer<T> {
  using detail::VectorBuffer<T>::nrows_;
  using detail::VectorBuffer<T>::ncols_;
  using detail::VectorBuffer<T>::data_;

  struct Row {
    T* row_;

    /** Second [] */
    T& operator[](std::size_t pos) { return row_[pos]; }
    const T& operator[](std::size_t pos) const { return row_[pos]; }
  };

 public:
  Matrix(std::size_t rows, std::size_t cols, T value = T())
      : detail::VectorBuffer<T>(rows, cols) {
    for (std::size_t i = 0; i < nrows_ * ncols_; ++i) {
      detail::construct(data_ + i, value);
    }
  }

  /** Creates matrix from given sequence */
  template <typename It>
  Matrix(std::size_t rows, std::size_t cols, It begin, It end)
      : detail::VectorBuffer<T>(rows, cols) {
    for (std::size_t i = 0; i < rows * cols && begin != end; ++i, ++begin) {
      detail::construct(data_ + i, *begin);
    }
  }

  Matrix(const Matrix& other)
      : detail::VectorBuffer<T>(other.nrows(), other.ncols()) {
    for (std::size_t i = 0; i < nrows_ * ncols_; ++i) {
      detail::construct(data_ + i, other[i]);
    }
  }

  Matrix& operator=(const Matrix& other) {
    Matrix tmp(other);
    std::swap(*this, tmp);
    return *this;
  }

  Matrix(Matrix&& other) noexcept = default;
  Matrix& operator=(Matrix&& other) noexcept = default;

  /** First [] */
  Row operator[](std::size_t row) { return Row(data_ + ncols_ * row); }

  /** Creates eye matrix */
  static Matrix eye(std::size_t n) {
    Matrix mx(n, n, 0);
    for (std::size_t i = 0; i < n; ++i) {
      mx[i][i] = 1;
    }
  }

  std::size_t nrows() const { return nrows_; }
  std::size_t ncols() const { return ncols_; }

  bool equal(const Matrix& other) const {
    return nrows_ == other.nrows_ && ncols_ && other.ncols_
        && std::equal(data_, data_ + nrows_ * ncols_, other.data_);
  }

  Matrix& negate() & {
    std::for_each(data_, data_ + nrows_ * ncols_, [](T& e){ e = -e; });
  }

  Matrix& transpose() & {
    Matrix tmp(*this);
    tmp.ncols_ = nrows_;
    tmp.nrows_ = ncols_;

    for (std::size_t i = 0; i < nrows_; ++i) {
      for (std::size_t j = 0; j < ncols_; ++j) {
        tmp[j][i] = *this[i][j];
      }
    }
    std::swap(*this, tmp);
  }

  T det() const {

  }

 private:

 public:
  using Iterator = IteratorBase<false>;
  using ConstIterator = IteratorBase<true>;

  Iterator begin() const { return Iterator(data_); }
  Iterator end() const { return Iterator(data_ + nrows_ * ncols_); }

  ConstIterator cbegin() const { return ConstIterator(data_); }
  ConstIterator cend() const { return ConstIterator(data_ + nrows_ * ncols_); }
};

} // namespace matrix
