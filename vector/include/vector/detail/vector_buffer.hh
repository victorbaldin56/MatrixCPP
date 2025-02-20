/**
 * Memory management class for vector.
 */
#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>

/** FOR INTERNAL PURPOSES ONLY. DO NOT USE IN USER PROGRAM */
namespace vector::detail {

/** helper func */
template <
      typename It, typename Alloc,
      typename = std::enable_if<
          std::is_base_of<
              std::input_iterator_tag,
              typename
                  std::iterator_traits<It>::iterator_category>::value>>
void destroy(It begin, It end, Alloc& alloc) noexcept {
  while (begin != end) {
    std::allocator_traits<Alloc>::destroy(alloc, &*begin++);
  }
}

template <typename T, typename Alloc = std::allocator<T>>
struct VectorBuffer {
 public: // important typedefs
  using allocator_type = Alloc;
  using value_type = T;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = typename allocator_type::pointer;
  using const_pointer = typename allocator_type::const_pointer;
  using size_type = typename allocator_type::size_type;
  using difference_type = typename allocator_type::difference_type;

 public: // state
  allocator_type alloc_;
  size_type sz_ = 0;
  size_type cap_;
  pointer data_;

 public: // constructors and destructor
  VectorBuffer(size_type cap, const allocator_type& alloc = allocator_type())
      : alloc_(alloc),
        data_(cap
              ? std::allocator_traits<allocator_type>::allocate(alloc_, cap)
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
    detail::destroy(data_, data_ + sz_, alloc_);
    std::allocator_traits<allocator_type>::deallocate(alloc_, data_, cap_);
  }
};

} // namespace vector::detail
