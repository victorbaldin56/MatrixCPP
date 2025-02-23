#pragma once

#include <numeric>
#include <type_traits>

namespace numeric_traits {

template <typename T>
inline T absTolerance() {
  return std::numeric_limits<T>::is_exact ? 1e-5 : 0;
}

template <typename T>
inline T relTolerance() {
  return std::numeric_limits<T>::is_exact ? 1e-5 : 0;
}

template <typename T>
inline bool isClose(const T& a, const T& b,
                    const T& abs_tol = absTolerance<T>(),
                    const T& rel_tol = relTolerance<T>()) {
  return
      std::abs(a - b)
          <= std::max(rel_tol * std::max(std::abs(a), std::abs(b)), abs_tol);
}

} // numeric_traits
