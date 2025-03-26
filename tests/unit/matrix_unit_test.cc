#include <stdexcept>

#include "gtest/gtest.h"
#include "matrix/matrix.hh"

TEST(matrix_ctor, simple) {
  // clang-format off
  std::vector<double> v1{ 1,  2,  3,  4,
                          5,  6,  7,  8,
                          9, 10, 11, 12,
                         13, 14, 15, 16};
  // clang-format on
  matrix::Matrix<double> m(4, 4, v1.begin());
  std::vector<double> v2(16);
  std::copy(m.begin(), m.end(), v2.begin());
  ASSERT_EQ(v1, v2);
}

TEST(matrix_ctor, ilist) {
  // clang-format off
  std::initializer_list<int> il{1, 2, 3, 4, 5, 6,
                                1, 2, 3, 4, 5, 6,
                                1, 2, 3, 4, 5, 6,
                                1, 2, 3, 4, 5, 6,
                                1, 2, 3, 4, 5, 6,
                                1, 2, 3, 4, 5, 6};
  // clang-format on
  std::vector<int> v(il);
  vector::Vector<int> vc(il.begin(), il.end());
  matrix::Matrix<int> m(6, 6, il.begin());
  ASSERT_TRUE(std::equal(v.begin(), v.end(), m.begin()));
}

TEST(det, simple) {
  // clang-format off
  std::vector<double> v{1,  2, 3,
                        4,  5, 6,
                        7, 87, 9};
  // clang-format on
  matrix::Matrix<double> m(3, 3, v.begin());
  ASSERT_TRUE(comparator::isClose(m.det(), 474.0));
}

TEST(det, large_values_matrix) {
  matrix::Matrix<double> m(10, 10, 0.0);
  // Matrix with large diagonal elements
  for (size_t i = 0; i < 10; ++i) {
    m[i][i] = 1e6 * (i + 1);
  }
  double expected = 1.0;
  for (size_t i = 1; i <= 10; ++i) expected *= 1e6 * i;
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
  matrix::Matrix<int> m;  // matrix with size 0
  try {
    m.det();
  } catch (std::runtime_error& ex) {
    return;
  }
  ASSERT_TRUE(0);
}

TEST(det, integer_matrix) {
  auto il0 = {1, 2, 2, 1};
  matrix::Matrix<int> m0(2, 2, il0.begin());
  ASSERT_TRUE(comparator::isClose(m0.det(), -3.0));

  auto il1 = {1, 2, 3, 4, 5, 6, 7, 87, 9};
  matrix::Matrix<int> m1(3, 3, il1.begin());
  ASSERT_TRUE(comparator::isClose(m1.det(), 474.0));

  // clang-format off
  auto il2 = {1, 2, 3, 4, 5,  6,
              2, 3, 4, 5, 6,  7,
              3, 5, 7, 9, 11, 13,
              0, 6, 7, 8, -3, -4,
              1, 3, 4, 9, -7, -2,
              0, 0, 0, 0, -9, 10};
  // clang-format on
  matrix::Matrix<int> m2(6, 6, il2.begin());
  ASSERT_TRUE(comparator::isClose(m2.det(), 0.0));

  // clang-format off
  auto il3 = {12,  2,   -170, 1742,  -8,  -2, -9, -10, -3, 5,
              8,   2,   -17,  536,   -2,  0,  -2, -3,  -1, 1,
              22,  -8,  -306, 1608,  -14, 3,  -5, -7,  1,  3,
              8,   -4,  -34,  0,     -2,  2,  1,  0,   1,  -1,
              20,  -2,  -153, 1139,  -8,  3,  -3, -6,  0,  2,
              58,  -6,  -459, 3283,  -23, 10, -8, -17, 0,  6,
              -24, -12, -51,  -1675, 3,   2,  7,  10,  5,  -3,
              -38, 2,   493,  -4154, 23,  1,  19, 23,  5,  -11,
              -2,  0,   17,   -335,  1,   1,  2,  2,   1,  -1,
              6,   -2,  -68,  402,   -3,  1,  -1, -2,  0,  1};
  // clang-format on
  matrix::Matrix<int> m3(10, 10, il3.begin());
  ASSERT_TRUE(comparator::isClose(m3.det(), 4556.0));
}

TEST(mmult, integer_matrix_2x2) {
  // clang-format off
  matrix::Matrix<int> m1{
    {1,  2},
    {10, 4}};

  matrix::Matrix<int> m2{
    {100, 5},
    {7,   8}};

  matrix::Matrix<int> ref{
    {114,  21},
    {1028, 82}};
  // clang-format on

  auto res = m1 * m2;
  ASSERT_TRUE(res == ref);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
