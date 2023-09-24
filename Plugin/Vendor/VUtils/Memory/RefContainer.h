#pragma once

#include <cstdint>

namespace Memory {
template <class T> class RefContainer {
public:
  explicit RefContainer(T *instance) {
    Instance = instance;
    Count++;
  }
  ~RefContainer() { delete Instance; }
  T *Instance{nullptr};
  std::uint64_t Count{0};
  std::uint64_t WeakCount{0};
};
} // namespace Memory