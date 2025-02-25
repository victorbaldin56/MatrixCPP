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
          std::is_base_of<
              std::input_iterator_tag,
              typename
                  std::iterator_traits<It>::iterator_category>::value>>
void destroy(It begin, It end) noexcept {
  while (begin != end) {
    destroy(std::addressof(*begin++));
  }
}
/** } */

template <typename T>
struct VectorBuffer {
 public: // important typedefs
  using value_type = T;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;

 public: // state
  size_type sz_ = 0;
  size_type cap_;
  pointer data_;

 public: // constructors and destructor
  VectorBuffer(size_type cap)
      : data_(cap
              ? static_cast<T*>(::operator new(cap * sizeof(value_type)))
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
