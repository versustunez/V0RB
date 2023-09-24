#pragma once

#include <cstddef>
#include <memory>

namespace Memory {
template <class T> class Scope {
public:
  Scope() = default;
  explicit Scope(T *instance) { m_Instance = instance; }
  ~Scope() { delete m_Instance; }
  Scope(Scope &other) = delete;
  Scope(Scope &&other) noexcept {
    m_Instance = other.m_Instance;
    other.m_Instance = nullptr;
  }
  T *Release() {
    T *tmp = m_Instance;
    m_Instance = nullptr;
    return tmp;
  }
  void Reset() {
    delete m_Instance;
    m_Instance = nullptr;
  }
  Scope &operator=(Scope other) {
    delete m_Instance;
    m_Instance = other.m_Instance;
    other.m_Instance = nullptr;
    return *this;
  }
  Scope &operator=(std::unique_ptr<T> other) {
    delete m_Instance;
    m_Instance = other.release();
    return *this;
  }
  T *operator->() const { return m_Instance; }
  T *operator*() const { return m_Instance; }
  T &operator*() { return *m_Instance; }
  T *Get() { return m_Instance; }
  template <class... Args> void Create(Args... args) {
    m_Instance = new T(args...);
  }
  template <class... Args> static Scope<T> Make(Args... args) {
    Scope<T> scope;
    scope.Create(args...);
    return scope;
  }

  bool operator==(const std::nullptr_t check) { return m_Instance == check; }
  explicit operator bool() { return m_Instance != nullptr; }

private:
  T *m_Instance{nullptr};
};
} // namespace Memory