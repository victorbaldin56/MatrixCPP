/**
 * Partly reimplemented std::vector
 * (there was an assignment requirement not to use std::vector).
 */
#pragma once

#include <initializer_list>
#include <iterator>

#include "detail/vector_buffer.hh"

namespace vector {

/** As just std::vector, no virtual destructors. */
template <typename T, typename Alloc = std::allocator<T>>
class Vector : private detail::VectorBuffer<T, Alloc> {
 private:
  /** iterator to the vector */
  template <bool IsConst>
  struct IteratorBase {
    using iterator_category = std::random_access_iterator_tag;
    using difference_type
        = typename detail::VectorBuffer<T, Alloc>::difference_type;
    using size_type = typename detail::VectorBuffer<T, Alloc>::size_type;
    using value_type = T;
    using pointer
        = typename
              std::conditional<
                  IsConst,
                  typename detail::VectorBuffer<T, Alloc>::const_pointer,
                  typename detail::VectorBuffer<T, Alloc>::pointer>::type;
    using reference
        = typename
              std::conditional<
                  IsConst,
                  typename detail::VectorBuffer<T, Alloc>::const_reference,
                  typename detail::VectorBuffer<T, Alloc>::reference>::type;

    explicit IteratorBase(pointer p) { ptr_ = p; }

    reference operator*() { return *ptr_; }
    pointer operator->() { return ptr_; }

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

    reference operator[](size_type pos) { return ptr_[pos]; }

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
    pointer ptr_;
  };

 public: // member types
  using iterator = IteratorBase<false>;
  using const_iterator = IteratorBase<true>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  using typename detail::VectorBuffer<T, Alloc>::allocator_type;
  using typename detail::VectorBuffer<T, Alloc>::size_type;
  using typename detail::VectorBuffer<T, Alloc>::difference_type;
  using typename detail::VectorBuffer<T, Alloc>::value_type;
  using typename detail::VectorBuffer<T, Alloc>::reference;
  using typename detail::VectorBuffer<T, Alloc>::const_reference;
  using typename detail::VectorBuffer<T, Alloc>::pointer;
  using typename detail::VectorBuffer<T, Alloc>::const_pointer;

 public: // constructors
  explicit Vector(size_type sz, const_reference val,
                  const allocator_type& alloc = allocator_type())
      : detail::VectorBuffer<value_type>(sz, alloc) {
    while (sz_ < cap_) {
      pushBack(val);
    }
  }

  explicit Vector(size_type sz = 0,
                  const allocator_type& alloc = allocator_type())
      : Vector(sz, value_type(), alloc) {}

  explicit Vector(const allocator_type& alloc = allocator_type())
      : Vector(0, value_type(), alloc) {}

  template <
      typename It,
      typename = std::enable_if<
          std::is_base_of<
              std::input_iterator_tag,
              typename
                  std::iterator_traits<It>::iterator_category>::value>>
  Vector(It begin, It end, const allocator_type& alloc = allocator_type())
      : detail::VectorBuffer<value_type>(std::distance(begin, end), alloc) {
    std::for_each(begin, end, [this](auto&& v) { pushBack(v); });
  }

  Vector(std::initializer_list<value_type> ilist,
         const allocator_type& alloc = allocator_type())
      : Vector(ilist.begin(), ilist.end(), alloc) {}

  Vector& operator=(std::initializer_list<value_type> ilist) {
    clear();
    reserve(ilist.size());
    std::for_each(ilist.begin(), ilist.end(),
                  [this](auto&& v){ pushBack(v); });
  }

  Vector(Vector&& rhs) noexcept = default;
  Vector& operator=(Vector&& rhs) noexcept = default;

  Vector(const Vector& rhs)
      : detail::VectorBuffer<value_type>(rhs.sz_, rhs.alloc_) {
    while (sz_ < rhs.sz_) {
      std::allocator_traits<allocator_type>::construct(
          alloc_, data_ + sz_, rhs.data_[sz_]);
      ++sz_;
    }
  }

  Vector& operator=(const Vector& rhs) {
    Vector tmp(rhs);
    std::swap(*this, tmp);
    return *this;
  }

 public: // iterators
  iterator begin() noexcept { return iterator(data_); }
  iterator end() noexcept { return iterator(data_ + sz_); }
  const_iterator cbegin() noexcept { return const_iterator(data_); }
  const_iterator cend() noexcept { return const_iterator(data_ + sz_); }

  reverse_iterator rbegin() noexcept {
    return reverse_iterator(end());
  }
  reverse_iterator rend() noexcept {
    return reverse_iterator(begin());
  }
  const_reverse_iterator crbegin() noexcept {
    return const_reverse_iterator(cend());
  }
  const_reverse_iterator crend() noexcept {
    return const_reverse_iterator(cbegin());
  }

 public: // capacity
  void reserve(size_type new_cap) {
    if (new_cap <= cap_) {
      return;
    }

    detail::VectorBuffer<T, Alloc> new_buf(new_cap, alloc_);
    while (new_buf.sz_ < sz_) {
      std::allocator_traits<allocator_type>::construct(
          alloc_, new_buf.data_ + new_buf.sz_, data_[new_buf.sz_]);
      ++new_buf.sz_;
    }

    // reserve does NOT change the size of the vector
    std::swap(new_buf.cap_, cap_);
    std::swap(new_buf.data_, data_);
  }

  size_type size() const noexcept { return sz_; }
  size_type capacity() const noexcept { return cap_; }

  bool empty() const noexcept { return sz_; }

 public: // accessors
  allocator_type getAllocator() { return alloc_; }

  pointer data() noexcept { return data_; }
  const pointer data() const noexcept { return data_; }

  reference front() { return *begin(); };
  reference back() { return *end(); }
  const_reference front() const { return *cbegin(); }
  const_reference back() const { return *cend(); }

  reference operator[](size_type pos) noexcept { return data_[pos]; }
  const_reference operator[](size_type pos) const noexcept {
    return data_[pos];
  }

 public: // modifiers
  void resize(size_type new_sz, const value_type& v = value_type()) {
    if (new_sz <= sz_) {
      detail::destroy(data_ + new_sz, data_ + sz_, alloc_);
      sz_ = new_sz;
      return;
    }

    reserve(new_sz);
    while (sz_ < new_sz) {
      pushBack(v);
    }
  }

  void pushBack(const_reference v) {
    auto tmp(v);
    pushBack(std::move(tmp));
  }

  void pushBack(value_type&& v) {
    if (sz_ == cap_) {
      reserve(getNextCap(cap_));
    }
    std::allocator_traits<allocator_type>::construct(
        alloc_, data_ + sz_, v);
    ++sz_;
  }

  void clear() noexcept {
    detail::destroy(begin(), end(), alloc_);
    sz_ = 0;
  }

  void popBack() noexcept {
    --sz_;
    std::allocator_traits<allocator_type>::destroy(alloc_, data_ + sz_);
  }

 private:
  static size_type getNextCap(size_type cap) noexcept {
    return (cap << 1) + 1;
  }

 private:
  using detail::VectorBuffer<T, Alloc>::alloc_;
  using detail::VectorBuffer<T, Alloc>::sz_;
  using detail::VectorBuffer<T, Alloc>::cap_;
  using detail::VectorBuffer<T, Alloc>::data_;
};

} // namespace vector
