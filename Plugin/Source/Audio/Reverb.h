#pragma once
#include "../Filter.h"
#include "Diffuser.h"
#include "MultiChannelFeedback.h"
#include "RingBuffer.h"
#include "VisualBuffer.h"

#include <array>

namespace VSTZ {
template <int channels = 8, int diffusionSteps = 4> class Reverb {
public:
  using DataArray = std::array<double, channels>;

  Reverb() : m_Diffuser(50) {
    m_Feedback.DecayFilter.SetFilterType(Filter::Type::HighShelf);
    m_Feedback.DecayFilter.Reset();
  }

  void SetRoomSize(double roomSize) {
    m_RoomSize = roomSize;
    m_Diffuser.SetSize(roomSize);
    m_IsDirty = true;
  }

  void SetDelayTime(double size) {
    m_DelayTime = size;
    m_IsDirty = true;
  }

  void UpdateFeedback() {
    m_Feedback.m_DelayTime = m_RoomSize * 0.001;
    constexpr float targetGain = 0.000001f;
    double averageDelayTime  = (m_RoomSize * 0.001) * 0.5;
    double invIterations  = (averageDelayTime) / m_DelayTime;
    m_Feedback.m_Decay = std::pow(targetGain, invIterations);
  }

  void SetDry(double dry) { m_Dry = dry; }
  void SetWet(double wet) { m_Wet = wet; }

  void Configure(double sampleRate) {
    if (!(sampleRate != m_SampleRate || m_IsDirty)) {
      return;
    }
    m_Feedback.DecayFilter.SetSampleRate(sampleRate);
    m_SampleRate = sampleRate;
    UpdateFeedback();
    m_IsDirty = false;
    m_Feedback.Configure(sampleRate);
    m_Diffuser.Configure(sampleRate);
  }

  double GetSampleRate() { return m_SampleRate; }

  DataArray Process(DataArray input) {
    auto diffuse = m_Diffuser.Process(input);
    auto longLasting = m_Feedback.Process(diffuse);
    DataArray output;
    for (int c = 0; c < channels; ++c) {
      output[c] = m_Wet * longLasting[c];
    }
    return output;
  }
  void ProcessStereo(float *leftData, float *rightData, int numSamples,
                     Ducker &ducker) {
    DataArray array;
    for (int i = 0; i < numSamples; ++i) {

      double duckerIn = std::max(fabs(leftData[i]), fabs(rightData[i]));
      double ducking = ducker.CalculateReduction(duckerIn);
      Buffer->Insert(duckerIn);

      float leftChannel = leftData[i];
      float rightChannel = rightData[i];

      for (int c = 0; c < channels; ++c) {
        // bit-and hack for equal or not equal.
        array[c] = ((c & 1) == 1) ? rightChannel : leftChannel;
      }
      array = Process(array);
      double left = 0, right = 0;
      for (int c = 0; c < channels; c += 2) {
        left += array[c];
        right += array[c + 1];
      }

      constexpr double scaling = 2.0 / channels;
      leftData[i] = (leftData[i] * m_Dry) + (left * scaling * ducking);
      rightData[i] = (rightData[i] * m_Dry) + (right * scaling * ducking);
    }
  }

  void SetBrightness(double brightness) {
    if (m_Brightness != brightness ||
        m_SampleRate != m_Feedback.DecayFilter.GetSampleRate()) {
      m_Feedback.DecayFilter.CalculateCoefficients(brightness, 12000, 0.707);
      m_Brightness = brightness;
    }
  }

  VisualBuffer* Buffer{nullptr};

protected:
  MultiChannelFeedback<channels> m_Feedback;
  DiffuserHalfLengths<channels, diffusionSteps> m_Diffuser;
  double m_Dry{0.5}, m_Wet{0.5};
  double m_RoomSize{0};
  double m_SampleRate{0};
  bool m_IsDirty{false};
  double m_DelayTime{50};
  double m_Brightness{50};

};
} // namespace VSTZ