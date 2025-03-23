#include <algorithm>
#include <list>
#include <vector>

#include "gtest/gtest.h"
#include "vector/vector.hh"

TEST(vector, size_constructor) {
  vector::Vector<int> custv(100, 2);
  std::vector<int> stdv(100, 2);
  ASSERT_TRUE(std::equal(custv.begin(), custv.end(), stdv.begin()));
  ASSERT_TRUE(std::equal(custv.cbegin(), custv.cend(), stdv.begin()));
  ASSERT_TRUE(std::equal(custv.rbegin(), custv.rend(), stdv.rbegin()));
  ASSERT_TRUE(std::equal(custv.crbegin(), custv.crend(), stdv.rbegin()));
}

TEST(vector, copy_constructor) {
  vector::Vector<int> v1(100, -1);
  vector::Vector<int> v2(v1);
  ASSERT_TRUE(std::equal(v1.begin(), v1.end(), v2.begin()));
  ASSERT_NE(v1.data(), v2.data());
  ASSERT_EQ(v1.size(), v2.size());
}

TEST(vector, two_iter_constructor) {
  std::list<int> l(100, -333);
  vector::Vector<int> v(l.begin(), l.end());
  ASSERT_TRUE(std::equal(l.begin(), l.end(), v.begin()));
}

TEST(vector, reserve) {
  vector::Vector<std::vector<int>> vv{
      {1, 2, 3, 4, 5},
      {2, 3, 4, 5, 0},
      {2, 3, 4, 89},
  };
  auto tmp = vv.data();

  std::vector<std::vector<int>> stdvv{
      {1, 2, 3, 4, 5},
      {2, 3, 4, 5, 0},
      {2, 3, 4, 89},
  };

  stdvv.resize(100000);

  ASSERT_EQ(vv.capacity(), 3);
  vv.reserve(100000);
  ASSERT_EQ(vv.capacity(), 100000);
  ASSERT_NE(vv.data(), tmp);

  tmp = vv.data();
  for (auto i = 3; i < 100000; ++i) {
    vv.push_back(std::vector<int>());
  }
  ASSERT_EQ(vv.data(), tmp);
  ASSERT_TRUE(std::equal(vv.rbegin(), vv.rend(), stdvv.rbegin()));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
