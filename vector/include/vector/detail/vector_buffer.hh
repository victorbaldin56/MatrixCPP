/**
 * Memory management class for vector.
 */
#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>

/**
 * FOR INTERNAL PURPOSES ONLY. DO NOT USE IN USER PROGRAM
 */
namespace vector::detail {

/**
 * @defgroup Helper functions {
 */
template <typename T, typename... Args>
void construct(T* p, Args&&... args) {
  new (p) T(std::forward<Args>(args)...);
}

template <typename T>
void destroy(T* p) noexcept {
  p->~T();
}

template <typename It,
          typename = std::enable_if_t<std::is_base_of_v<
              std::input_iterator_tag,
              typename std::iterator_traits<It>::iterator_category>>>
void destroy(It begin, It end) noexcept {
  while (begin != end) {
    destroy(std::addressof(*begin++));
  }
}
/** } */

template <typename T>
struct VectorBuffer {
 public:  // state
  std::size_t sz_ = 0;
  std::size_t cap_;
  T* data_;

 public:  // constructors and destructor
  explicit VectorBuffer(std::size_t cap)
      : data_(cap ? static_cast<T*>(::operator new(cap * sizeof(T))) : nullptr),
        cap_(cap) {}

  VectorBuffer(const VectorBuffer& other) = delete;
  VectorBuffer& operator=(const VectorBuffer& other) = delete;

  VectorBuffer(VectorBuffer&& other) noexcept
      : sz_(std::exchange(other.sz_, 0)),
        cap_(std::exchange(other.cap_, 0)),
        data_(std::exchange(other.data_, nullptr)) {}

  VectorBuffer& operator=(VectorBuffer&& other) noexcept {
    sz_ = std::exchange(other.sz_, 0);
    cap_ = std::exchange(other.cap_, 0);
    data_ = std::exchange(other.data_, 0);
    return *this;
  }

  ~VectorBuffer() {
    detail::destroy(data_, data_ + sz_);
    ::operator delete(data_);
  }
};

}  // namespace vector::detail
