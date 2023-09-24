#pragma once

#include "Delay.h"
#include "Matrix.h"

#include <array>

namespace VSTZ {
template <int channels = 8> struct MultiChannelFeedback {
  using DataArray = std::array<double, channels>;

public:
  void Configure(double sampleRate) {
    double delaySamplesBase = m_DelayTime * sampleRate;
    for (int c = 0; c < channels; ++c) {
      double r = c * 1.0 / channels;
      m_DelaySamples[c] = std::pow(2, r) * delaySamplesBase;
      m_Delays[c].Resize(m_DelaySamples[c] + 1);
      m_Delays[c].Reset();
    }
  }

  DataArray Process(DataArray input) {
    DataArray delayed;
    for (int c = 0; c < channels; ++c) {
      delayed[c] = DecayFilter.Apply(m_Delays[c].Read(m_DelaySamples[c]), c);
    }
    DataArray mixed = delayed;
    Householder<channels>::InPlace(mixed.data());
    for (int c = 0; c < channels; ++c) {
      double sum = input[c] + mixed[c] * m_Decay;
      m_Delays[c].Write(sum);
    }
    return delayed;
  }

  double m_Decay = 0.85;
  double m_DelayTime = 0.150;

  Filter DecayFilter;

protected:
  std::array<int, channels> m_DelaySamples;
  std::array<VSTZ::Delay, channels> m_Delays;
};
} // namespace VSTZ