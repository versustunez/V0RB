#pragma once

#include <random>

namespace VSTZ {
struct Random {
  explicit Random(double low = 0.0, double high = 1.0) : m_Dist(low, high) {}

  double Get(double low, double hi) {
    std::uniform_real_distribution<double> dist(low, hi);
    return dist(m_MT);
  }

  double Get() { return m_Dist(m_MT); }

protected:
  std::random_device m_RD;
  std::mt19937 m_MT{m_RD()};
  std::uniform_real_distribution<double> m_Dist;
};
} // namespace VSTZ