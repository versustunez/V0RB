#pragma once

#include <cmath>

namespace VSTZ {
template <int size> class Householder {
  static constexpr double Multiplier{-2.0 / size};

public:
  static void InPlace(double *arr) {
    double sum = 0;
    for (int i = 0; i < size; ++i) {
      sum += arr[i];
    }

    sum *= Multiplier;

    for (int i = 0; i < size; ++i) {
      arr[i] += sum;
    }
  };
};

template <int size> class Hadamard {
public:
  static inline void RecursiveUnscaled(double *data) {
    if (size <= 1)
      return;
    constexpr int hSize = size / 2;

    Hadamard<hSize>::RecursiveUnscaled(data);
    Hadamard<hSize>::RecursiveUnscaled(data + hSize);

    // Combine the two halves using sum/difference
    for (int i = 0; i < hSize; ++i) {
      double a = data[i];
      double b = data[i + hSize];
      data[i] = (a + b);
      data[i + hSize] = (a - b);
    }
  }

  static inline void InPlace(double *data) {
    RecursiveUnscaled(data);
    double scalingFactor = std::sqrt(1.0 / size);
    for (int c = 0; c < size; ++c) {
      data[c] *= scalingFactor;
    }
  }
};
} // namespace VSTZ