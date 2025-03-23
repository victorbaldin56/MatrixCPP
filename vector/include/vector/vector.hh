/**
 * Partly reimplemented std::vector
 * (there was an assignment requirement not to use std::vector).
 */
#pragma once

#include <initializer_list>
#include <utility>

#include "detail/iterator_base.hh"
#include "detail/vector_buffer.hh"

namespace vector {

/**
 * Custom std::vector.
 */
template <typename T>
class Vector final : private detail::VectorBuffer<T> {
 public:  // member types
  /**
   * @defgroup Iterators {
   */
  using iterator = detail::IteratorBase<T>;
  using const_iterator = detail::IteratorBase<const T>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  /** } */

  /**
   * @defgroup Iterator traits {
   */
  using difference_type = typename iterator::difference_type;
  using value_type = typename iterator::value_type;
  using reference = typename iterator::reference;
  using pointer = typename iterator::pointer;
  /** } */

  /**
   * @defgroup Self traits {
   */
  using size_type = std::size_t;
  using const_reference = const value_type&;
  using const_pointer = const value_type*;
  /** } */

 public:  // constructors
  explicit Vector(size_type sz = 0, const_reference val = value_type())
      : detail::VectorBuffer<value_type>(sz) {
    std::fill_n(std::back_inserter(*this), sz, val);
  }

  template <typename It,
            typename = std::enable_if_t<std::is_base_of_v<
                std::input_iterator_tag,
                typename std::iterator_traits<It>::iterator_category>>>
  Vector(It begin, It end)
      : detail::VectorBuffer<value_type>(std::distance(begin, end)) {
    std::copy(begin, end, std::back_inserter(*this));
  }

  Vector(std::initializer_list<value_type> ilist)
      : Vector(ilist.begin(), ilist.end()) {}

  Vector(Vector&& rhs) noexcept = default;
  Vector& operator=(Vector&& rhs) noexcept = default;

  Vector(const Vector& rhs) : detail::VectorBuffer<value_type>(rhs.sz_) {
    std::copy(rhs.cbegin(), rhs.cend(), std::back_inserter(*this));
  }

  Vector& operator=(const Vector& rhs) {
    Vector tmp(rhs);
    std::swap(*this, tmp);
    return *this;
  }

 public:  // iterators
  iterator begin() noexcept { return iterator(data_); }
  iterator end() noexcept { return iterator(data_ + sz_); }
  const_iterator cbegin() const noexcept { return const_iterator(data_); }
  const_iterator cend() const noexcept { return const_iterator(data_ + sz_); }

  reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
  reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
  const_reverse_iterator crbegin() const noexcept {
    return const_reverse_iterator(cend());
  }
  const_reverse_iterator crend() const noexcept {
    return const_reverse_iterator(cbegin());
  }

 public:  // capacity
  void reserve(size_type new_cap) {
    if (new_cap <= cap_) {
      return;
    }

    detail::VectorBuffer<value_type> new_buf(new_cap);
    while (new_buf.sz_ < sz_) {
      detail::construct(new_buf.data_ + new_buf.sz_, data_[new_buf.sz_]);
      ++new_buf.sz_;
    }

    // reserve does NOT change the size of the vector
    std::swap(new_buf.cap_, cap_);
    std::swap(new_buf.data_, data_);
  }

  size_type size() const noexcept { return sz_; }
  size_type capacity() const noexcept { return cap_; }

  bool empty() const noexcept { return sz_; }

 public:  // accessors
  pointer data() noexcept { return data_; }
  const_pointer data() const noexcept { return data_; }

  reference front() { return *begin(); };
  reference back() { return *end(); }
  const_reference front() const { return *cbegin(); }
  const_reference back() const { return *cend(); }

  reference operator[](size_type pos) noexcept { return data_[pos]; }
  const_reference operator[](size_type pos) const noexcept {
    return data_[pos];
  }

 public:  // modifiers
  void resize(size_type new_sz, const value_type& v = value_type()) {
    if (new_sz <= sz_) {
      detail::destroy(data_ + new_sz, data_ + sz_);
      sz_ = new_sz;
      return;
    }

    reserve(new_sz);
    std::fill_n(std::back_inserter(*this), new_sz - sz_, v);
  }

  template <typename... Args>
  void emplace_back(Args&&... args) {
    if (sz_ == cap_) {
      reserve(getNextCap(cap_));
    }
    detail::construct(data_ + sz_, std::forward<Args>(args)...);
    ++sz_;
  }

  void push_back(value_type&& v) { emplace_back(v); }

  void push_back(const_reference v) { emplace_back(v); }

  void clear() noexcept {
    detail::destroy(begin(), end());
    sz_ = 0;
  }

  void pop_back() noexcept {
    --sz_;
    detail::destroy(data_ + sz_);
  }

 private:
  static size_type getNextCap(size_type cap) noexcept { return (cap << 1) + 1; }

 private:
  using detail::VectorBuffer<T>::sz_;
  using detail::VectorBuffer<T>::cap_;
  using detail::VectorBuffer<T>::data_;
};

}  // namespace vector
