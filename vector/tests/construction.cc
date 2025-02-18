#include <algorithm>
#include <vector>

#include "gtest/gtest.h"

#include "vector/vector.hh"

TEST(vector_constructor, simple) {
  vector::Vector<int> v1(100, 1);
  std::vector<int> v2(100, 1);
  ASSERT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
}

TEST(vector_constructor, initializer_list) {
  vector::Vector<int> v1{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  std::vector<int> v2{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  ASSERT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
}

TEST(vector_constructor, copy) {
  vector::Vector<std::vector<int>> vv1{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  auto vv2 = vv1;
  ASSERT_TRUE(std::equal(vv1.begin(), vv1.end(), vv2.begin()));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
