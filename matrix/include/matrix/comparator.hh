#pragma once

#include <numeric>

namespace comparator {

template <typename T>
constexpr T kAbsTol =
    std::numeric_limits<T>::is_exact ? static_cast<T>(0)
                                     : static_cast<T>(1e-10);

template <typename T>
constexpr T kRelTol =
    std::numeric_limits<T>::is_exact ? static_cast<T>(0)
                                     : static_cast<T>(1e-10);

template <typename T>
inline bool isClose(const T& a, const T& b,
                    const T& abs_tol = kAbsTol<T>,
                    const T& rel_tol = kRelTol<T>) noexcept {
  return std::abs(a - b) <=
         std::max(rel_tol * std::max(std::abs(a), std::abs(b)), abs_tol);
}

template <typename T>
inline bool isLessClose(const T& a, const T& b,
                        const T& abs_tol = kAbsTol<T>,
                        const T& rel_tol = kRelTol<T>) noexcept {
  return isClose(a, b, abs_tol, rel_tol) || a < b;
}

template <typename T>
inline bool isGreaterClose(const T& a, const T& b,
                           const T& abs_tol = kAbsTol<T>,
                           const T& rel_tol = kRelTol<T>) noexcept {
  return isClose(a, b, abs_tol, rel_tol) || a > b;
}

}  // namespace comparator
