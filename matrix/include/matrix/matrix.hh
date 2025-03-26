/**
 * Matrix implementation from scratch.
 */
#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <type_traits>

#include "comparator.hh"
#include "vector/vector.hh"

namespace matrix {

template <typename T>
class Matrix final {
  static_assert(std::is_arithmetic_v<T>);

  // Contigious storage chosen here because of
  // 1. Positive attitude to cache effects.
  // 2. Less dynamic memory allocations.
  // 3. Less indirections.
  using ContigiousContainer =
      typename vector::Vector<T>;  ///< stores matrix data

 public:  // member types
  using iterator = typename ContigiousContainer::iterator;
  using const_iterator = typename ContigiousContainer::const_iterator;
  using reverse_iterator = typename ContigiousContainer::reverse_iterator;
  using const_reverse_iterator =
      typename ContigiousContainer::const_reverse_iterator;
  using value_type = typename ContigiousContainer::value_type;
  using reference = typename ContigiousContainer::reference;
  using const_reference = typename ContigiousContainer::const_reference;
  using pointer = typename ContigiousContainer::pointer;
  using difference_type = typename ContigiousContainer::difference_type;
  using size_type = typename ContigiousContainer::size_type;

 public:  // constructors
  Matrix() {}

  /** Creates and fills matrix with given value */
  Matrix(size_type rows, size_type cols, const_reference val = value_type())
      : data_(rows * cols, val), rows_(rows), cols_(cols) {}

  /** Creates matrix from given sequence */
  template <typename It,
            typename = std::enable_if_t<std::is_base_of_v<
                std::input_iterator_tag,
                typename std::iterator_traits<It>::iterator_category>>>
  Matrix(size_type rows, size_type cols, It begin) : rows_(rows), cols_(cols) {
    auto sz = rows_ * cols_;
    data_.reserve(sz);
    std::copy_n(begin, sz, std::back_inserter(data_));
  }

  template <typename U>
  Matrix(const Matrix<U>& other) : rows_(other.rows()), cols_(other.cols()) {
    data_.reserve(rows_ * cols_);
    std::copy(other.cbegin(), other.cend(), std::back_inserter(data_));
  }

  Matrix(const std::initializer_list<std::initializer_list<T>>& ilist) {
    rows_ = ilist.size();
    if (!rows_) {
      return;
    }

    cols_ = ilist.begin()->size();
    if (!cols_) {
      throw std::runtime_error(
          "Cannot initialize matrix with rows != 0 and cols = 0");
    }

    data_.reserve(rows_ * cols_);

    std::for_each(ilist.begin(), ilist.end(), [this](auto&& row) {
      if (row.size() != cols_) {
        throw std::runtime_error("Invalid initializer list");
      }
      std::copy(row.begin(), row.end(), std::back_inserter(data_));
    });
  }

 private:
  template <bool IsConst>
  class ProxyRowBase {
    using StoredIterator =
        typename std::conditional_t<IsConst, const_iterator, iterator>;
    using RowElemReference =
        typename std::conditional_t<IsConst, const_reference, reference>;

    StoredIterator begin_;
    size_type cols_;

   public:
    ProxyRowBase(StoredIterator p, size_type cols) noexcept
        : begin_(p), cols_(cols) {}

    RowElemReference operator[](size_type pos) { return begin_[pos]; }

    StoredIterator begin() noexcept { return begin_; }
    StoredIterator end() noexcept { return begin_ + cols_; }

    const_iterator cbegin() const noexcept { return begin_; }
    const_iterator cend() const noexcept { return begin_ + cols_; }
  };

 public:  // member types
  using ProxyRow = ProxyRowBase<false>;
  using ConstProxyRow = ProxyRowBase<true>;

 public:  // accessors
  ProxyRow operator[](size_type pos) noexcept {
    return ProxyRow(data_.begin() + pos * cols_, cols_);
  }

  ConstProxyRow operator[](size_type pos) const noexcept {
    return ConstProxyRow(data_.cbegin() + pos * cols_, cols_);
  }

  size_type rows() const noexcept { return rows_; }
  size_type cols() const noexcept { return cols_; }
  pointer data() noexcept { return data_.data(); }
  const pointer data() const noexcept { return data_.data(); }

  bool isSquare() const noexcept { return rows_ == cols_; }

 public:  // iterators
  iterator begin() noexcept { return data_.begin(); }
  iterator end() noexcept { return data_.end(); }
  const_iterator cbegin() const noexcept { return data_.cbegin(); }
  const_iterator cend() const noexcept { return data_.cend(); }

  reverse_iterator rbegin() noexcept { return data_.rbegin(); }
  reverse_iterator rend() noexcept { return data_.rend(); }
  const_reverse_iterator crbegin() const noexcept { return data_.crbegin(); }
  const_reverse_iterator crend() const noexcept { return data_.crend(); }

 public:  // modifiers
  void resize(size_type new_rows, size_type new_cols) {
    if (!new_cols || !new_rows) {
      throw std::runtime_error("Resizing to null matrix is not allowed");
    }

    rows_ = new_rows;
    cols_ = new_cols;
    data_.resize(rows_ * cols_);
  }

  void setRows(size_type new_rows) { resize(new_rows, cols_); }
  void setCols(size_type new_cols) { resize(rows_, new_cols); }

  bool swapRows(size_type a, size_type b) noexcept {
    if (a == b) {
      return false;
    }
    auto arow = operator[](a);
    std::swap_ranges(arow.begin(), arow.end(), operator[](b).begin());
    return true;
  }

  // currently supports only floating point calculations
  void simplifyRows(size_type idx) {
    auto base_row = operator[](idx);
    auto base_elem = base_row[idx];
    assert(!comparator::isClose(base_elem, static_cast<value_type>(0)));
    auto base_row_begin = base_row.begin();
    auto base_row_end = base_row.end();

    for (auto j = idx + 1; j < rows_; ++j) {
      auto cur_row = operator[](j);
      auto cur_row_begin = cur_row.begin();
      auto coef = cur_row[idx] / base_elem;
      std::transform(
          base_row_begin, base_row_end, cur_row_begin, cur_row_begin,
          [coef](const auto& a, const auto& b) { return b - coef * a; });
    }
  }

 public:  // computing functions
  double det() const {
    if (!isSquare()) {
      throw std::runtime_error("Matrix::det(): rows_ != cols_");
    }

    if (!rows_ || !cols_) {
      throw std::runtime_error("Matrix::det(): matrix size must be > 0");
    }

    auto calc_matrix = Matrix<double>(*this);
    auto sign = 1.0;
    for (size_type i = 0; i < cols_; ++i) {
      auto pivot = i;
      for (auto j = i + 1; j < rows_; ++j) {
        if (std::abs(calc_matrix[j][i]) > std::abs(calc_matrix[pivot][i])) {
          pivot = j;
        }
      }

      if (calc_matrix.swapRows(i, pivot)) {
        sign = -sign;
      }

      if (comparator::isClose<double>(calc_matrix[i][i], 0)) {
        return 0;
      }
      calc_matrix.simplifyRows(i);
    }

    auto det = sign;
    for (size_type i = 0; i < rows_; ++i) {
      auto&& elem = calc_matrix[i][i];
      assert(std::isfinite(elem));
      det *= elem;
    }
    return det;
  }

 public:
  auto operator==(const Matrix& rhs) const { return data_ == rhs.data_; }
  auto operator!=(const Matrix& rhs) const { return !(*this == rhs); }

  Matrix& operator*=(const Matrix& other);

 public:  // static functions
  /** Creates eye matrix */
  static Matrix eye(size_type n) {
    Matrix m(n, n);
    for (auto i = size_type{0}; i < n; ++i) {
      m[i][i] = static_cast<value_type>(1);
    }
    return m;
  }

 private:
  size_type rows_ = 0;
  size_type cols_ = 0;
  ContigiousContainer data_;
};

template <typename T>
auto operator*(const Matrix<T>& a, const Matrix<T>& b) {
  auto a_cols = a.cols();
  auto a_rows = a.rows();
  auto b_cols = b.cols();
  auto b_rows = b.rows();

  if (a_cols != b_rows) {
    throw std::runtime_error("product: invalid arguments (a.cols != b.rows)");
  }

  Matrix<T> res(a_cols, b_rows);
  using size_type = typename Matrix<T>::size_type;
  for (size_type i = 0; i < a_rows; ++i) {
    auto a_row_i = a[i];
    for (size_type j = 0; j < b_cols; ++j) {
      for (size_type k = 0; k < a_cols; ++k) {
        res[i][j] += a_row_i[k] * b[k][j];
      }
    }
  }
  return res;
}

template <typename T>
Matrix<T>& Matrix<T>::operator*=(const Matrix<T>& other) {
  auto res = *this * other;
  std::swap(res, *this);
  return *this;
}

}  // namespace matrix
