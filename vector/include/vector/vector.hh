/**
 * Partly reimplemented std::vector
 * (there was an assignment requirement not to use std::vector).
 */
#pragma once

#include "detail/vector_buffer.hh"

namespace vector {

template <typename T>
class Vector : private detail::VectorBuffer<T> {
  using detail::VectorBuffer<T>::sz_;
  using detail::VectorBuffer<T>::cap_;
  using detail::VectorBuffer<T>::data_;

 public:
  explicit Vector(std::size_t sz = 0, const T& val = T())
      : detail::VectorBuffer<T>(sz) {
    while (sz_ < cap_) {
      detail::construct(data_ + sz_, val);
      ++sz_;
    }
  }

  Vector(Vector&& rhs) noexcept = default;
  Vector& operator=(Vector&& rhs) noexcept = default;

  Vector(const Vector& rhs)
      : detail::VectorBuffer<T>(rhs.sz_) {
    while (sz_ < rhs.sz_) {
      detail::construct(data_[sz_], rhs.data_[sz_]);
      ++sz_;
    }
  }

  Vector& operator=(const Vector& rhs) {
    Vector tmp(rhs);
    std::swap(*this, tmp);
    return *this;
  }

 private:
  /** iterator to the vector */
  template <bool IsConst>
  struct IteratorBase {
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;

    using pointer = typename std::conditional<IsConst, const T*, T*>::type;
    using reference = typename std::conditional<IsConst, const T&, T&>::type;

    explicit IteratorBase(pointer p) { ptr_ = p; }

    T& operator*() { return ptr_; }
    T* operator->() { return ptr_; }

    // prefix increment
    IteratorBase& operator++() {
      ++ptr_;
      return *this;
    }

    // postfix increment
    IteratorBase operator++(int) {
      IteratorBase tmp = *this;
      ++*this;
      return tmp;
    }

    // prefix decrement
    IteratorBase& operator--() {
      --ptr_;
      return *this;
    }

    // postfix decrement
    IteratorBase operator--(int) {
      IteratorBase tmp = *this;
      --*this;
      return tmp;
    }

    IteratorBase operator+(difference_type n) const {
      return IteratorBase(ptr_ + n);
    }

    IteratorBase operator-(difference_type n) const {
      return IteratorBase(ptr_ - n);
    }

    difference_type operator-(const IteratorBase& other) const {
      return ptr_ - other.ptr_;
    }

    IteratorBase& operator+=(difference_type n) {
      ptr_ += n;
      return *this;
    }

    IteratorBase& operator-=(difference_type n) {
      ptr_ -= n;
      return *this;
    }

    bool operator==(const IteratorBase& other) const {
      return ptr_ == other.ptr_;
    }

    bool operator!=(const IteratorBase& other) const {
      return ptr_ != other.ptr_;
    }

    bool operator<(const IteratorBase& other) const {
      return ptr_ < other.ptr_;
    }

    bool operator>(const IteratorBase& other) const {
      return ptr_ > other.ptr_;
    }

    bool operator<=(const IteratorBase& other) const {
      return ptr_ <= other.ptr_;
    }

    bool operator>=(const IteratorBase& other) const {
      return ptr_ >= other.ptr_;
    }

   private:
    T* ptr_;
  };

  /** reverse iterator to the vector */
  template <bool IsConst>
  struct ReverseIteratorBase {
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;

    using pointer = typename std::conditional<IsConst, const T*, T*>::type;
    using reference = typename std::conditional<IsConst, const T&, T&>::type;

    explicit ReverseIteratorBase(pointer p) { ptr_ = p; }

    T& operator*() { return ptr_; }
    T* operator->() { return ptr_; }

    // prefix increment
    ReverseIteratorBase& operator++() {
      --ptr_;
      return *this;
    }

    // postfix increment
    ReverseIteratorBase operator++(int) {
      IteratorBase tmp = *this;
      --*this;
      return tmp;
    }

    // prefix decrement
    ReverseIteratorBase& operator--() {
      ++ptr_;
      return *this;
    }

    // postfix decrement
    ReverseIteratorBase operator--(int) {
      IteratorBase tmp = *this;
      ++*this;
      return tmp;
    }

    ReverseIteratorBase operator+(difference_type n) const {
      return ReverseIteratorBase(ptr_ - n);
    }

    ReverseIteratorBase operator-(difference_type n) const {
      return ReverseIteratorBase(ptr_ + n);
    }

    difference_type operator-(const ReverseIteratorBase& other) const {
      return other.ptr_ - ptr_;
    }

    ReverseIteratorBase& operator+=(difference_type n) {
      ptr_ -= n;
      return *this;
    }

    ReverseIteratorBase& operator-=(difference_type n) {
      ptr_ -= n;
      return *this;
    }

    bool operator==(const ReverseIteratorBase& other) const {
      return ptr_ == other.ptr_;
    }

    bool operator!=(const ReverseIteratorBase& other) const {
      return ptr_ != other.ptr_;
    }

    bool operator<(const ReverseIteratorBase& other) const {
      return ptr_ > other.ptr_;
    }

    bool operator>(const ReverseIteratorBase& other) const {
      return ptr_ < other.ptr_;
    }

    bool operator<=(const ReverseIteratorBase& other) const {
      return ptr_ >= other.ptr_;
    }

    bool operator>=(const ReverseIteratorBase& other) const {
      return ptr_ <= other.ptr_;
    }

   private:
    T* ptr_;
  };

 public:
  using Iterator = IteratorBase<false>;
  using ConstIterator = IteratorBase<true>;
  using ReverseIterator = ReverseIteratorBase<false>;
  using ConstReverseIterator = ReverseIteratorBase<true>;

 public:
  auto begin() { return Iterator(data_); }
  auto end() { return Iterator(data_ + sz_); }
  auto cbegin() { return ConstIterator(data_); }
  auto cend() { return ConstIterator(data_ + sz_); }

  auto rbegin() { return ReverseIterator(data_ + sz_ - 1); }
  auto rend() { return ReverseIterator(data_ - 1); }
  auto crbegin() { return ConstReverseIterator(data_ + sz_ - 1); }
  auto crend() { return ConstReverseIterator(data_ - 1); }

 public:
  auto size() const { return sz_; }
  auto capacity() const { return cap_; }
  auto data() const { return data_; }

 public:
  T& operator[](std::size_t pos) { return data_[pos]; }
  const T& operator[](std::size_t pos) const { return data_[pos]; }

 public:
  void reserve(std::size_t new_cap) {
    detail::VectorBuffer<T> new_buf(new_cap);
    while (new_buf.sz_ < sz_) {
      detail::construct(new_buf.data_ + new_buf.sz_, data_[new_buf.sz_]);
      ++new_buf.sz_ ;
    }
  }

};

} // namespace vector
