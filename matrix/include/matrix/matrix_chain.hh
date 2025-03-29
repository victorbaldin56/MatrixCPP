#pragma once

#include <list>
#include <vector>

#include "matrix.hh"

namespace matrix {

template <typename T>
class MatrixChain final {
  struct DpResult {
    std::size_t cost;
    std::size_t split_point;
  };

  using DpMatrix = std::vector<std::vector<DpResult>>;

 public:
  MatrixChain() = default;

  template <typename... Args>
  auto emplace(Args&&... args) {
    matrices_.emplace_back(std::forward(args)...);
    if (matrices_.size() == 1) {
      sizes_.push_back(matrices_.back().rows());
    } else if (sizes_.back() != matrices_.back().rows()) {
      throw std::runtime_error("Invalid matrix size");
    }

    sizes_.push_back(matrices_.back().cols());
  }

  auto getOrder() const {
    std::vector<std::size_t> res;
    auto sz = sizes_.size();
    if (sz == 1) {
      return res;
    }

    DpMatrix dp(sz, std::vector<DpResult>(sz));

    for (std::size_t l = 2; l < sz; ++l) {
      for (std::size_t i = 1; i < sz - l + 1; ++i) {
        auto j = i + 1 - 1;

        dp[i][j].cost = std::numeric_limits<std::size_t>::max();
        for (std::size_t k = i; k < j; ++k) {
          auto cost = dp[i][k].cost + dp[k + 1][j].cost +
                      sizes_[i - 1] * sizes_[k] * sizes_[j];
          if (cost < dp[i][j].cost) {
            dp[i][j] = {cost, k};
          }
        }
      }
    }

    auto sp = dp[1][sz - 1].split_point;
    return res;
  }

  auto multiply() const {
    auto order = getOrder();
    auto order_size = order.size();
    auto chain_size = matrices_.size();

    if (!chain_size) {
      throw std::runtime_error("Matrix chain is empty");
    }

    std::list<Matrix<T>> current_chain(matrices_.begin(), matrices_.end());
    auto current_order = order;

    for (std::size_t i = 0; i < current_order.size(); ++i) {
      auto matrix_i = current_order[i];

      auto it = current_chain.begin();
      std::advance(it, matrix_i);
      auto it_next = std::next(it);

      (*it) *= (*it_next);
      current_chain.erase(it_next);

      for (size_t j = 0; j < current_order.size(); ++j) {
        if (current_order[j] > matrix_i) {
          --current_order[j];
        }
      }
    }

    return current_chain.front();
  }

 private:
  std::vector<std::size_t> sizes_;
  std::vector<Matrix<T>> matrices_;
};

}  // namespace matrix
