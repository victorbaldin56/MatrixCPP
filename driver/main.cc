#include <cstdlib>
#include <iostream>
#include <iterator>

#include "matrix/matrix.hh"

int main() try {
  auto n = *std::istream_iterator<std::size_t>(std::cin);
  matrix::Matrix<double> m(n, n);
  std::copy_n(std::istream_iterator<double>(std::cin), n * n, m.begin());
  if (!std::cin.good()) {
    throw std::runtime_error("Unexpected EOF");
  }
  std::cout << m.det() << std::endl;
  return 0;
} catch (std::exception& ex) {
  std::cerr << ex.what() << std::endl;
  return EXIT_FAILURE;
}
