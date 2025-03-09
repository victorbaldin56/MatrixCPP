#pragma once

#include <cstddef>
#include <iterator>

namespace vector::detail {

template <typename ValueType>
struct IteratorBase {
  using iterator_category = std::random_access_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;
  using value_type = ValueType;
  using pointer
      = typename std::conditional<
                std::is_const_v<value_type>,
                const value_type*,
                value_type*>::type;
  using const_pointer = const value_type*;
  using reference
      = typename std::conditional<
            std::is_const_v<value_type>,
            const value_type&,
            value_type&>::type;
  using const_reference = const value_type&;

  explicit IteratorBase(pointer p) noexcept : ptr_(p) {};

  reference operator*() noexcept { return *ptr_; }
  pointer operator->() noexcept { return ptr_; }

  // prefix increment
  IteratorBase& operator++() noexcept {
    ++ptr_;
    return *this;
  }
  // postfix increment
  IteratorBase operator++(int) noexcept {
    IteratorBase tmp = *this;
    ++*this;
    return tmp;
  }
  // prefix decrement
  IteratorBase& operator--() noexcept {
    --ptr_;
    return *this;
  }
  // postfix decrement
  IteratorBase operator--(int) noexcept {
    IteratorBase tmp = *this;
    --*this;
    return tmp;
  }
  IteratorBase operator+(difference_type n) const noexcept {
    return IteratorBase(ptr_ + n);
  }
  IteratorBase operator-(difference_type n) const noexcept {
    return IteratorBase(ptr_ - n);
  }
  difference_type operator-(const IteratorBase& other) const noexcept {
    return ptr_ - other.ptr_;
  }
  IteratorBase& operator+=(difference_type n) noexcept {
    ptr_ += n;
    return *this;
  }
  IteratorBase& operator-=(difference_type n) noexcept {
    ptr_ -= n;
    return *this;
  }

  reference operator[](size_type pos) noexcept { return ptr_[pos]; }

  bool operator==(const IteratorBase& other) const noexcept {
    return ptr_ == other.ptr_;
  }
  bool operator!=(const IteratorBase& other) const noexcept {
    return ptr_ != other.ptr_;
  }
  bool operator<(const IteratorBase& other) const noexcept {
    return ptr_ < other.ptr_;
  }
  bool operator>(const IteratorBase& other) const noexcept {
    return ptr_ > other.ptr_;
  }
  bool operator<=(const IteratorBase& other) const noexcept {
    return ptr_ <= other.ptr_;
  }
  bool operator>=(const IteratorBase& other) const noexcept {
    return ptr_ >= other.ptr_;
  }

 private:
  pointer ptr_;
};

}  // namespace vector::detail
