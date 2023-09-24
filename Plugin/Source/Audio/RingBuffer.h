#pragma once
#include <cstddef>
#include <mutex>

namespace VSTZ {
template <typename T, size_t Size = 8192> class RingBuffer {
public:
  constexpr static size_t Mask = Size - 1;

  RingBuffer() {
    static_assert((Size & Mask) == 0, "Size must be a power of two");
    for (auto &it : m_Buffer) {
      it = {};
    }
  }

  void Insert(T Value) {
    m_Buffer[m_Index] = Value;
    m_Index = (m_Index + 1) & Mask;
  }

private:
  T m_Buffer[Size];
  std::size_t m_Index = 0;

public:
  class Iterator {
  public:
    Iterator(RingBuffer &Buffer, std::size_t Index)
        : m_Buffer(Buffer),
          m_Index(Index),
          m_Remaining(Size) {}

    T &operator*() { return m_Buffer.m_Buffer[m_Index]; }

    T *operator->() { return &m_Buffer.m_Buffer[m_Index]; }

    Iterator &operator++() {
      --m_Remaining;
      m_Index = (m_Index + 1) & m_Buffer.Mask;
      return *this;
    }

    bool operator==(const Iterator &) const { return m_Remaining == 0; }

    bool operator!=(const Iterator &) const { return m_Remaining > 0; }

  private:
    RingBuffer &m_Buffer;
    std::size_t m_Index;
    unsigned m_Remaining;
  };

  Iterator begin() { return Iterator(*this, m_Index); }

  Iterator end() { return Iterator(*this, m_Index); }
};

template <typename T> using RingBuffer512 = VSTZ::RingBuffer<T, 512>;
template <typename T> using RingBuffer1024 = VSTZ::RingBuffer<T, 1024>;
template <typename T> using RingBuffer2048 = VSTZ::RingBuffer<T, 2048>;
template <typename T> using RingBuffer4096 = VSTZ::RingBuffer<T, 4096>;
template <typename T> using RingBuffer8192 = VSTZ::RingBuffer<T, 8192>;
template <typename T> using RingBuffer16384 = VSTZ::RingBuffer<T, 8192 * 2>;
} // namespace V3D
