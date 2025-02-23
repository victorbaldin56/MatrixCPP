#include <cstdlib>
#include <cstdint>

#include "boost/process.hpp"

#include "gtest/gtest.h"

#include "matrix/matrix.hh"

TEST(det, integer_matrix) {
  // run generator python script
  boost::process::ipstream pipe_stream;
  pipe_stream.exceptions(
      boost::process::ipstream::failbit | boost::process::ipstream::eofbit);
  boost::process::child c("python3 ./generate_data.py",
                          boost::process::std_out > pipe_stream);
  while (!pipe_stream.eof()) {
    std::size_t n;
    pipe_stream >> n;
    matrix::Matrix<std::int64_t> m(n, n);
    std::transform(m.begin(), m.end(), m.begin(),
                   [&pipe_stream](){
                     std::int64_t v;
                     pipe_stream >> v;
                     return v;
                   });
    std::int64_t ans;
    pipe_stream >> ans;
    ASSERT_EQ(ans, m.det());
  }
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

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
