/**
 * Memory management class for vector.
 */
#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>

/** FOR INTERNAL PURPOSES ONLY. DO NOT USE IN USER PROGRAM */
namespace vector::detail {

/**
 * @defgroup Helper functions {
 */
template <typename T>
void construct(T* p, T&& v) { new(p) T(std::move(v)); }

template <typename T>
void construct(T* p, const T& v) { new(p) T(v); }

template <typename T>
void destroy(T* p) noexcept { p->~T(); }

template <
      typename It,
      typename = std::enable_if<
          std::is_base_of_v<
              std::input_iterator_tag,
              typename
                  std::iterator_traits<It>::iterator_category>>>
void destroy(It begin, It end) noexcept {
  while (begin != end) {
    destroy(std::addressof(*begin++));
  }
}
/** } */

template <typename T>
struct VectorBuffer {
 public: // state
  std::size_t sz_ = 0;
  std::size_t cap_;
  T* data_;

 public: // constructors and destructor
  explicit VectorBuffer(std::size_t cap)
      : data_(cap
              ? static_cast<T*>(::operator new(cap * sizeof(T)))
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
