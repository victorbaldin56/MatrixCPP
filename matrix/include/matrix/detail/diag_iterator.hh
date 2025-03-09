#pragma once

#include <iterator>

namespace matrix::detail {

template <typename Iter, typename Size>
struct DiagIterBase {
  using iterator_category = std::random_access_iterator_tag;
  using difference_type = typename Iter::difference_type;
  using value_type = typename Iter::value_type;
  using reference = typename Iter::reference;
  using pointer = typename Iter::pointer;

  DiagIterBase(Iter iter, Size cols) noexcept
      : iter_(iter), step_(cols + 1) {}

  reference operator*() noexcept { return *iter_; }
  pointer operator->() noexcept { return iter_->operator->(); }

  // prefix increment
  DiagIterBase& operator++() noexcept {
    iter_ += step_;
    return *this;
  }
  // postfix increment
  DiagIterBase operator++(int) noexcept {
    auto tmp = *this;
    ++*this;
    return tmp;
  }
  // prefix decrement
  DiagIterBase operator--() noexcept {
    iter_ -= step_;
    return *this;
  }
  // postfix decrement
  DiagIterBase operator--(int) noexcept {
    auto tmp = *this;
    --*this;
    return tmp;
  }
  DiagIterBase operator+(difference_type n) const noexcept {
    return DiagIterBase(iter_ + n * step_);
  }
  DiagIterBase operator-(difference_type n) const noexcept {
    return DiagIterBase(iter_ - n * step_);
  }
  difference_type operator-(const DiagIterBase& other) const noexcept {
    return (iter_ - other.iter_) / step_;
  }
  DiagIterBase& operator+=(difference_type n) noexcept {
    iter_ += n * step_;
    return *this;
  }
  DiagIterBase& operator-=(difference_type n) noexcept {
    iter_ -= n * step_;
    return *this;
  }

  reference operator[](difference_type pos) noexcept {
    return iter_[pos * step_];
  }

  auto operator<=>(const DiagIterBase& other) {
    return iter_ <=> other.iter_;
  }

 private:
  Iter iter_;
  Size step_;
};

}  // namespace matrix::detail
