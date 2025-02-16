/**
 * Memory management class for vector.
 */
#pragma once

#include <algorithm>
#include <cstddef>
#include <new>

namespace vector::detail {

template <typename T> void construct(T *p, const T &rhs) { new (p) T(rhs); }
template <typename T> void construct(T *p, T &&rhs) {
  new (p) T(std::move(rhs));
}

template <typename T>
inline void destroy(T* p) { p->~T(); }

template <typename It>
void destroy(It begin, It end) {
  while (begin != end) {
    destroy(&*begin++);
  }
}

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
    destroy(data_, data_ + sz_);
    ::operator delete(data_);
  }
};

} // namespace vector::detail
