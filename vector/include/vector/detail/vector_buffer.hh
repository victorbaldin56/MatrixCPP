/**
 * Memory management class for vector.
 */
#pragma once

#include <algorithm>
#include <cstddef>
#include <new>

/** FOR INTERNAL PURPOSES ONLY. DO NOT USE IN USER PROGRAM */
namespace vector::detail {

// TODO: завернуть бы в аллокатор
template <typename T>
T* allocate(std::size_t n) {
  return static_cast<T*>(::operator new(n * sizeof(T)));
}

template <typename T>
void construct(T *p, const T &rhs) { new (p) T(rhs); }

template <typename T>
void construct(T *p, T &&rhs) {
  new (p) T(std::move(rhs));
}

template <typename T>
inline void destroy(T* p) noexcept { p->~T(); }

template <
      typename It,
      typename = std::enable_if<
          std::is_pointer<It>::value &&
          std::is_base_of<
              std::input_iterator_tag,
              typename
                  std::iterator_traits<It>::iterator_category>::value>>
void destroy(It begin, It end) noexcept {
  while (begin != end) {
    destroy(&*begin++);
  }
}

template <typename T>
class VectorBuffer {
 public:
  std::size_t sz_ = 0;
  std::size_t cap_;
  T* data_;

 public:
  VectorBuffer(std::size_t cap)
      : data_(cap ? allocate<T>(cap)
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
    detail::destroy(data_, data_ + sz_);
    ::operator delete(data_);
  }
};

} // namespace vector::detail
