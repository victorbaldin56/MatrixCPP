#pragma once

#include <utility>

namespace mm {

template <typename T>
inline void construct(T *p, const T &rhs) { new (p) T(rhs); }

template <typename T>
inline void construct(T *p, T &&rhs) {
  new (p) T(std::move(rhs));
}

template <typename T>
inline void destroy(T* p) { p->~T(); }

template <typename It>
void destroy(It begin, It end) {
  while (begin != end) {
    destroy(&*begin++);
  }
}

} // namespace mm
