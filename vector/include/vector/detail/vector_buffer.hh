/**
 * Memory management class for vector.
 */
#pragma once

#include <algorithm>
#include <cstddef>
#include <new>

#include "mm/mm.hh"

namespace vector::detail {

template <typename T>
class VectorBuffer {
 protected:
  std::size_t sz_ = 0;
  std::size_t cap_;
  T* data_;

 protected:
  VectorBuffer(std::size_t cap)
      : data_(cap ? static_cast<T*>(::operator new(cap * sizeof(T)))
                  : nullptr),
        cap_(cap) {}

  VectorBuffer(const VectorBuffer& other) = delete;
  VectorBuffer& operator=(const VectorBuffer& other) = delete;

  VectorBuffer(VectorBuffer&& other) noexcept
      : sz_(other.sz_), cap_(other.cap_), data_(other.data_) {
    other.sz_ = 0;
    other.cap_ = 0;
    other.data_ = nullptr;
  }

  VectorBuffer& operator=(VectorBuffer&& other) noexcept {
    std::swap(sz_, other.sz_);
    std::swap(cap_, other.cap_);
    std::swap(data_, other.data_);
    return *this;
  }

  ~VectorBuffer() {
    mm::destroy(data_, data_ + sz_);
    ::operator delete(data_);
  }
};

} // namespace vector::detail
