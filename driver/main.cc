#include <cstdlib>
#include <iostream>
#include <iterator>

#include "matrix/matrix.hh"

namespace {

void checkInputStream(std::istream& s) {
  if (!s.good()) {
    throw std::runtime_error("Unexpected EOF");
  }
}

}

int main() try {
  std::size_t n;
  std::cin >> n;
  checkInputStream(std::cin);
  matrix::Matrix<double> m(n, n);
  if (n != 0) {
    std::copy_n(std::istream_iterator<double>(std::cin), n * n, m.begin());
    checkInputStream(std::cin);
  }
  std::cout << m.det() << std::endl;
  return 0;
} catch (std::exception& ex) {
  std::cerr << ex.what() << std::endl;
  return EXIT_FAILURE;
}
