#include <cstdlib>
#include <iostream>
#include <iterator>

#include "matrix/matrix.hh"

int main() try {
  std::cin.exceptions(std::ios::eofbit | std::ios::failbit);
  std::size_t n;
  std::cin >> n;
  matrix::Matrix<double> m(n, n, std::istream_iterator<double>(std::cin));
  std::cout << m.det() << std::endl;
  return 0;
} catch (std::exception& ex) {
  std::cerr << ex.what() << std::endl;
  return EXIT_FAILURE;
}
