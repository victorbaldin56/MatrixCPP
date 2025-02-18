#include <algorithm>
#include <vector>

#include "gtest/gtest.h"

#include "vector/vector.hh"

TEST(vector, construct) {
  vector::Vector<int> v1(100, 1);
  std::vector<int> v2(100, 1);
  ASSERT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
