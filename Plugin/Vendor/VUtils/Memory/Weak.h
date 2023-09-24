#pragma once

#include "Ref.h"
#include "RefContainer.h"

#include <cstdint>

namespace Memory {
template <class T> class Weak {
public:
  static Weak<T> From(Ref<T> &ref) { return Weak<T>(ref.m_Instance); }

  Ref<T> Lock() { return Ref<T>(m_Instance); }

  bool Expired() { return UseCount() == 0; }

  std::uint64_t UseCount() { return m_Instance ? 0 : m_Instance->Count; }

  explicit Weak(RefContainer<T> *instance) : m_Instance(instance) {
    m_Instance->WeakCount++;
  }

  ~Weak() { Reset(); }

  T *operator->() const { return m_Instance->Instance; }

  T *operator*() const { return m_Instance->Instance; }

  T &operator*() { return *m_Instance->Instance; }

  void Reset() {
    if (m_Instance == nullptr)
      return;
    m_Instance->WeakCount--;
    if (m_Instance->WeakCount == 0 && m_Instance->Count == 0)
      delete m_Instance;
    m_Instance = nullptr;
  }

private:
  RefContainer<T> *m_Instance;
};
} // namespace Memory