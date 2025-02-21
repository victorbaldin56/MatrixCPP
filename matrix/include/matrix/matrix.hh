/**
 * Matrix implementation from scratch.
 */
#pragma once

#include <algorithm>
#include <cstddef>
#include <cmath>
#include <stdexcept>
#include <type_traits>

#include "vector/vector.hh"

#include "numeric_traits.hh"

namespace matrix {

template <
    typename T, typename Alloc = std::allocator<T>,
    typename = std::enable_if<
        std::is_convertible_v<int, T> &&
        std::is_same<
            decltype(-std::declval<T>()), T>::value &&
        std::is_same<
            decltype(std::declval<T>() + std::declval<T>()), T>::value &&
        std::is_same<
            decltype(std::declval<T>() - std::declval<T>()), T>::value &&
        std::is_same<
            decltype(std::declval<T>() * std::declval<T>()), T>::value &&
        std::is_same<
            decltype(std::declval<T>() / std::declval<T>()), T>::value>>
class Matrix {
  // Contigious storage chosen here because of
  // 1. Positive attitude to cache effects.
  // 2. Less dynamic memory allocations.
  // 3. Less indirections.
  using ContigiousContainer
      = typename vector::Vector<T, Alloc>; /** stores matrix data */

 public: // member types
  using allocator_type = typename ContigiousContainer::allocator_type;
  using iterator = typename ContigiousContainer::iterator;
  using const_iterator = typename ContigiousContainer::const_iterator;
  using reverse_iterator = typename ContigiousContainer::reverse_iterator;
  using const_reverse_iterator
      = typename ContigiousContainer::const_reverse_iterator;
  using value_type = typename ContigiousContainer::value_type;
  using reference = typename ContigiousContainer::reference;
  using const_reference = typename ContigiousContainer::const_reference;
  using pointer = typename ContigiousContainer::pointer;
  using difference_type = typename ContigiousContainer::difference_type;
  using size_type = typename ContigiousContainer::size_type;

 public: // constructors
  /** Creates and fills matrix with given value */
  Matrix(size_type rows = 0, size_type cols = 0, const_reference val = value_type(),
         const allocator_type& alloc = allocator_type())
      : data_(rows * cols, val, alloc), rows_(rows), cols_(cols) {}

  /** Creates matrix from given sequence */
  template <
      typename It,
      typename = std::enable_if<
          std::is_base_of<
              std::input_iterator_tag,
              typename
                  std::iterator_traits<It>::iterator_category>::value>>
  Matrix(size_type rows, size_type cols, It begin, It end,
         const allocator_type& alloc = allocator_type())
      : rows_(rows), cols_(cols),
        data_(rows * cols, alloc) { std::copy(begin, end, data_.begin()); }

  template <
      typename It,
      typename = std::enable_if<
          std::is_base_of<
              std::input_iterator_tag,
              typename
                  std::iterator_traits<It>::iterator_category>::value>>
  Matrix(size_type cols, It begin, It end,
         const allocator_type& alloc = allocator_type())
      : data_(begin, end, alloc),
        cols_(cols),
        rows_(std::ceil(static_cast<double>(data_.size()) / cols)) {}

  Matrix(size_type cols, std::initializer_list<value_type> ilist,
         const allocator_type& alloc = allocator_type())
      : Matrix(cols, ilist.begin(), ilist.end(), alloc) {}

  virtual ~Matrix() {}

 private:
  template <bool IsConst>
  class ProxyRowBase {
    using StoredIterator
        = typename std::conditional<IsConst, const_iterator, iterator>::type;

    StoredIterator begin_;
    size_type cols_;

   public:
    ProxyRowBase(StoredIterator p, size_type cols) noexcept : begin_(p), cols_(cols) {}

    template <typename = std::enable_if<!IsConst>>
    reference operator[](size_type pos) { return begin_[pos]; }

    const_reference operator[](size_type pos) const { return begin_[pos]; }

    template <typename = std::enable_if<!IsConst>>
    StoredIterator begin() { return begin_; }

    template <typename = std::enable_if<!IsConst>>
    StoredIterator end() { return begin_ + cols_; }

    const_iterator cbegin() const { return begin_; }
    const_iterator cend() const { return begin_ + cols_; }
  };

 public: // member types
  using ProxyRow = ProxyRowBase<false>;
  using ConstProxyRow = ProxyRowBase<true>;

 public: // accessors
  ProxyRow operator[](size_type pos) noexcept {
    return ProxyRow(data_.begin() + pos * cols_, cols_);
  }

  ConstProxyRow operator[](size_type pos) const noexcept {
    return ConstProxyRow(data_.begin() + pos * cols_, cols_);
  }

  size_type rows() const noexcept { return rows_; }
  size_type cols() const noexcept { return cols_; }
  pointer data() noexcept { return data_.data(); }
  const pointer data() const noexcept { return data_.data(); }

  bool isSquare() const noexcept { return rows_ == cols_; }

 public: // iterators
  iterator begin() noexcept { return data_.begin(); }
  iterator end() noexcept { return data_.end(); }
  const_iterator cbegin() const noexcept { return data_.cbegin(); }
  const_iterator cend() const noexcept { return data_.cend(); }

  reverse_iterator rbegin() noexcept { return data_.rbegin(); }
  reverse_iterator rend() noexcept { return data_.rend(); }
  const_reverse_iterator crbegin() const noexcept { return data_.crbegin(); }
  const_reverse_iterator crend() const noexcept { return data_.crend(); }

 public: // modifiers
  void resize(size_type new_rows, size_type new_cols) {
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

  void simplifyRows(size_type idx) {
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

 public: // computing functions
  value_type det() const {
    if (!isSquare()) {
      throw std::runtime_error("Matrix::det: rows_ != cols_");
    }

    if (!rows_ || !cols_) {
      throw std::runtime_error("Matrix:det: matrix size must be > 0");
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
        return static_cast<value_type>(0);
      }
      mcopy.simplifyRows(i);
    }

    auto det = static_cast<value_type>(sign);
    for (auto i = 0; i < rows_; ++i) {
      det *= mcopy[i][i];
    }
    return det;
  }

 public: // assignments
  Matrix& operator=(std::initializer_list<value_type> ilist) {
    auto sz = ilist.size();
    if (sz > rows_ * cols_) {
      setRows(std::ceil(static_cast<double>(sz) / cols_));
    }
    std::copy(ilist.begin(), ilist.end(), data_.begin());
    return *this;
  }

 public: // static functions
  /** Creates eye matrix */
  static Matrix eye(size_type n) {
    Matrix m(n, n);
    for (std::size_t i = 0; i < m.rows(); ++i) {
      m[i][i] = static_cast<T>(1);
    }
    return m;
  }

 private:
  ContigiousContainer data_;
  size_type rows_;
  size_type cols_;
};

} // namespace matrix
