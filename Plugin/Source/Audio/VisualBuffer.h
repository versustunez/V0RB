#pragma once

#include "RingBuffer.h"

#include <array>

namespace VSTZ {
struct VisualBuffer {
  static constexpr size_t Size{1024};
  RingBuffer<double, Size> Buffer;
  std::array<double, 128> PeakBuffer{};
  size_t Index{};
  void Insert(double value);
};
} // namespace VSTZ
