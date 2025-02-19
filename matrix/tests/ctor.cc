#include <vector>

#include "gtest/gtest.h"

#include "matrix/matrix.hh"

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

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
