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

TEST(matrix_ctor, simple) {
  std::vector<double> v1{1,  2,  3,   4,
                         5,  6,  7,   8,
                         9,  10, 11, 12,
                         13, 14, 15, 16};
  matrix::Matrix<double> m(4, v1.begin(), v1.end());
  std::vector<double> v2(16);
  std::copy(m.begin(), m.end(), v2.begin());
  ASSERT_EQ(v1, v2);
}

TEST(matrix_ctor, ilist) {
  std::initializer_list<int> il{1, 2, 3, 4, 5, 6,
                                1, 2, 3, 4, 5, 6,
                                1, 2, 3, 4, 5, 6,
                                1, 2, 3, 4, 5, 6,
                                1, 2, 3, 4, 5, 6,
                                1, 2, 3, 4, 5, 6};
  std::vector<int> v(il);
  matrix::Matrix<int> m(6, il);
  ASSERT_TRUE(std::equal(v.begin(), v.end(), m.begin()));
}

TEST(matrix_assign, ilist) {
  matrix::Matrix<double> m(5, 6);
  std::initializer_list<double> il{1, 2, 3, 4, 5, 6,
                                   1, 2, 3, 4, 5, 6,
                                   1, 2, 3, 4, 5, 6,
                                   1, 2, 3, 4, 5, 6,
                                   1, 2, 3, 4, 5, 6,
                                   1, 2, 3, 4, 5, 6};
  m = il;
  std::vector<double> v(il);
  ASSERT_EQ(m.cols(), 6);
  ASSERT_EQ(m.rows(), 6);
  ASSERT_TRUE(std::equal(v.begin(), v.end(), m.begin()));
}

TEST(det, simple) {
  std::vector<double> v{1,  2, 3,
                        4,  5, 6,
                        7, 87, 9};
  matrix::Matrix<double> m(3, v.begin(), v.end());
  ASSERT_DOUBLE_EQ(m.det(), 474);
}

TEST(det, eye) {
  auto m = matrix::Matrix<double>::eye(1000);
  ASSERT_DOUBLE_EQ(m.det(), 1);
}

TEST(det, zero_size) {
  matrix::Matrix<int> m; // matrix with size 0
  try {
    m.det();
  } catch (std::runtime_error& ex) {
    return;
  }
  ASSERT_TRUE(0);
}

TEST(det, integer_matrix) {
  matrix::Matrix<int> m0(
      2, {1, 2,
          2, 1});
  ASSERT_DOUBLE_EQ(m0.det(), -3);

  matrix::Matrix<int> m1(
      3, {1,  2, 3,
          4,  5, 6,
          7, 87, 9});
  ASSERT_DOUBLE_EQ(m1.det(), 474);

  matrix::Matrix<int> m2(
      6, {1, 2, 3, 4,  5,  6,
          2, 3, 4, 5,  6,  7,
          3, 5, 7, 9, 11, 13,
          0, 6, 7, 8, -3, -4,
          1, 3, 4, 9, -7, -2,
          0, 0, 0, 0, -9, 10});
  ASSERT_DOUBLE_EQ(m2.det(), 0);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
