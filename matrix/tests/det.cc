#include "gtest/gtest.h"

#include "matrix/matrix.hh"

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

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
