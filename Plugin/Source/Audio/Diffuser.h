#pragma once

#include "Delay.h"
#include "Matrix.h"
#include "Random.h"

#include <array>

namespace VSTZ {
template <int channels> class DiffusionStep {
protected:
  using DataArray = std::array<double, channels>;

public:
  void Configure(double sampleRate, Random &random) {
    double delayRange = DelayMs * 0.001 * sampleRate;
    for (int c = 0; c < channels; ++c) {
      double rangeLow = delayRange * c / channels;
      double rangeHigh = delayRange * (c + 1) / channels;
      m_DelaySamples[c] = random.Get(rangeLow, rangeHigh);
      m_Delays[c].Resize(m_DelaySamples[c] + 1);
      m_FlipPolarity[c] = random.Get(0, 1) > 0.5;
    }
  }

  DataArray Process(DataArray input) {
    DataArray delayed;
    for (int c = 0; c < channels; ++c) {
      m_Delays[c].Write(input[c]);
      delayed[c] = m_Delays[c].Read(m_DelaySamples[c]);
    }
    DataArray mixed = delayed;
    Hadamard<channels>::InPlace(mixed.data());
    for (int c = 0; c < channels; ++c) {
      if (m_FlipPolarity[c])
        mixed[c] *= -1;
    }

    return mixed;
  }

  double DelayMs{50};

protected:
  std::array<int, channels> m_DelaySamples;
  std::array<Delay, channels> m_Delays;
  std::array<bool, channels> m_FlipPolarity;
};

template <int channels = 8, int stepCount = 4> struct DiffuserHalfLengths {
  using DataArray = std::array<double, channels>;

  using Step = DiffusionStep<channels>;
  std::array<Step, stepCount> steps;
  Random random;

  DiffuserHalfLengths(double diffusionMs) { SetSize(diffusionMs); }

  void SetSize(double inMs) {
    for (auto &step : steps) {
      inMs *= 0.5;
      step.DelayMs = inMs;
    }
  }

  void Configure(double sampleRate) {
    for (auto &step : steps)
      step.Configure(sampleRate, random);
  }

  DataArray Process(DataArray samples) {
    for (auto &step : steps) {
      samples = step.Process(samples);
    }
    return samples;
  }
};
} // namespace VSTZ