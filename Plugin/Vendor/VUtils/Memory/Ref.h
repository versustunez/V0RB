#pragma once

#include "RefContainer.h"
#include "Scope.h"

#include <cstddef>
#include <cstdint>
#include <memory>

namespace Memory {
template <class T> class Weak;
template <class T> class Ref {
public:
  Ref() = default;

  explicit Ref(T *instance) { Allocate(instance); }
  explicit Ref(RefContainer<T> *instance) {
    m_Instance = instance;
    m_Instance->Count++;
  }

  Ref(const Ref &other) {
    m_Instance = other.m_Instance;
    m_Instance->Count++;
  }

  Ref(const Ref &&other) noexcept {
    m_Instance = other.m_Instance;
    m_Instance->Count++;
  }

  ~Ref() { Reset(); }

  T *operator->() const { return m_Instance->Instance; }

  T *Get() { return m_Instance->Instance; }

  T &operator*() { return *m_Instance->Instance; }

  Ref<T> &operator=(Ref<T> other) {
    Reset();
    m_Instance = other.m_Instance;
    m_Instance->Count++;
  }

  Ref<T> &operator=(Scope<T> other) {
    Reset();
    Allocate(other.Release());
    return *this;
  }

  void Reset() {
    if (m_Instance == nullptr)
      return;
    m_Instance->Count--;
    if (m_Instance->Count == 0)
      Free();
    else
      m_Instance = nullptr;
  }

  template <class... Args> void Create(Args... args) {
    Allocate(new T(args...));
  }

  template <class... Args> static Ref<T> Make(Args... args) {
    Ref<T> instance;
    instance.Create(args...);
    return instance;
  }

  Weak<T> AsWeak() { return Weak<T>::From(*this); }
  bool operator==(const std::nullptr_t check) { return m_Instance == check; }
  explicit operator bool() { return m_Instance != nullptr; }

private:
  void Allocate(T *instance) { m_Instance = new RefContainer<T>(instance); }

  void Free() {
    delete m_Instance->Instance;
    m_Instance->Instance = nullptr;
    if (m_Instance->WeakCount == 0)
      delete m_Instance;
  }

private:
  RefContainer<T> *m_Instance;
  friend Weak<T>;
};
} // namespace Memory