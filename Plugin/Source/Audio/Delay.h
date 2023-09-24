#pragma once

#include <vector>

namespace VSTZ {
class Buffer {
public:
  explicit Buffer(int minCapacity = 1) { Resize(minCapacity); }
  Buffer(const Buffer &other) = delete;
  Buffer &operator=(const Buffer &other) = delete;
  Buffer(Buffer &&other) = default;
  Buffer &operator=(Buffer &&other) = default;

  void Resize(int minCapacity = 1, double value = 0) {
    int bufferLength = 1;
    while (bufferLength < minCapacity)
      bufferLength *= 2;
    m_Buffer.assign(bufferLength, value);
    m_BufferMask = unsigned(bufferLength - 1);
    m_BufferIndex = 0;
  }
  void Reset(double value = 0) { m_Buffer.assign(m_Buffer.size(), value); }
  Buffer &operator++() {
    ++m_BufferIndex;
    return *this;
  }

  double &operator[](int offset) {
    return m_Buffer[(m_BufferIndex + (unsigned)offset) & m_BufferMask];
  }

protected:
  unsigned m_BufferIndex{0};
  unsigned m_BufferMask{0};
  std::vector<double> m_Buffer{};
};

class Delay {
public:
  void Reset() { m_Buffer.Reset(); }
  void Resize(int minCapacity) { m_Buffer.Resize(minCapacity); }

  double Read(int delaySamples) { return m_Buffer[-delaySamples]; }
  Delay &Write(double value) {
    ++m_Buffer;
    m_Buffer[0] = value;
    return *this;
  }

protected:
  Buffer m_Buffer;
};
} // namespace VSTZ