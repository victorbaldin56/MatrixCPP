#include <stdexcept>

#include "gtest/gtest.h"

#include "matrix/matrix.hh"

TEST(matrix_ctor, simple) {
  std::vector<double> v1{ 1,  2,  3,  4,
                          5,  6,  7,  8,
                          9, 10, 11, 12,
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
  ASSERT_TRUE(comparator::isClose(m.det(), 474.0));
}

TEST(det, large_values_matrix) {
  matrix::Matrix<double> m(10, 10, 0.0);
  // Matrix with large diagonal elements
  for(size_t i = 0; i < 10; ++i) {
    m[i][i] = 1e6 * (i + 1);
  }
  double expected = 1.0;
  for(size_t i = 1; i <= 10; ++i) expected *= 1e6 * i;
  ASSERT_TRUE(comparator::isClose(m.det(), expected));
}

TEST(det, near_singular_matrix) {
  matrix::Matrix<double> m = matrix::Matrix<double>::eye(10);
  // Create nearly singular matrix
  m[9][9] = 1e-30;
  ASSERT_TRUE(comparator::isClose(m.det(), 1e-30));
}

TEST(det, eye) {
  auto m = matrix::Matrix<double>::eye(1000);
  ASSERT_TRUE(comparator::isClose(m.det(), 1.0));
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
  ASSERT_TRUE(comparator::isClose(m0.det(), -3.0));

  matrix::Matrix<int> m1(
      3, {1,  2, 3,
          4,  5, 6,
          7, 87, 9});
  ASSERT_TRUE(comparator::isClose(m1.det(), 474.0));

  matrix::Matrix<int> m2(
      6, {1, 2, 3, 4,  5,  6,
          2, 3, 4, 5,  6,  7,
          3, 5, 7, 9, 11, 13,
          0, 6, 7, 8, -3, -4,
          1, 3, 4, 9, -7, -2,
          0, 0, 0, 0, -9, 10});
  ASSERT_TRUE(comparator::isClose(m2.det(), 0.0));

  matrix::Matrix<int> m3(
      10, {12, 2, -170, 1742, -8, -2, -9, -10, -3, 5,
           8, 2, -17, 536, -2, 0, -2, -3, -1, 1,
           22, -8, -306, 1608, -14, 3, -5, -7, 1, 3,
           8, -4, -34, 0, -2, 2, 1, 0, 1, -1,
           20, -2, -153, 1139, -8, 3, -3, -6, 0, 2,
           58, -6, -459, 3283, -23, 10, -8, -17, 0, 6,
           -24, -12, -51, -1675, 3, 2, 7, 10, 5, -3,
           -38, 2, 493, -4154, 23, 1, 19, 23, 5, -11,
           -2, 0, 17, -335, 1, 1, 2, 2, 1, -1,
           6, -2, -68, 402, -3, 1, -1, -2, 0, 1});
  ASSERT_TRUE(comparator::isClose(m3.det(), 4556.0));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
