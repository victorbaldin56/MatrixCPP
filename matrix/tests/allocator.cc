#include <stdexcept>

#include "gtest/gtest.h"

#include "matrix/matrix.hh"

template <typename T>
class ThrowingAllocator {
 public:
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using pointer = T*;
  using const_pointer = const T*;

 public:
  ThrowingAllocator() {}

 public:
  pointer allocate(size_type) { throw std::bad_alloc(); }
  void deallocate(pointer p, size_type n) {}
};

TEST(matrix, throwing_allocator) {
  try {
    matrix::Matrix<int, ThrowingAllocator<int>> m(1, 2);
  } catch (std::bad_alloc& ex) {
    return;
  }
  ASSERT_TRUE(0);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
