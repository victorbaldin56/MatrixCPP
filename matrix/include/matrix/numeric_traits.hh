#pragma once

#include <numeric>
#include <type_traits>

namespace numeric_traits {

template <typename T>
inline T epsilon() {
  return std::numeric_limits<T>::is_exact ? 1e-5 : 0;
}

template <typename T>
inline bool isClose(T a, T b) { return std::abs(a - b) <= epsilon<T>(); }

} // numeric_traits
