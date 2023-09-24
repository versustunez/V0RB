#include "Ducker.h"

#include <cmath>
#include <JuceHeader.h>

namespace VSTZ {
double Ducker::CalculateReduction(double in) {
  double dBsample = juce::Decibels::gainToDecibels(in);
  double gain = 1.0;
  if (dBsample > m_Threshold) {
    double overThreshold = dBsample - m_Threshold;
    double desiredChange = overThreshold / m_Ratio;
    gain = juce::Decibels::decibelsToGain(-abs(desiredChange - overThreshold));
  }
  double rate = gain > m_Envelope ? m_AttackRate : m_ReleaseRate;
  m_Envelope = m_Envelope * rate + (1.0 - rate) * gain;

  GainReductionBuffer->Insert(m_Envelope);
  return m_Envelope;
}
void Ducker::SetThreshold(double threshold) {
  m_Threshold = threshold;
}

constexpr double MaxReleaseMs = 150 * 0.001;
constexpr double MinReleaseMs = 5 * 0.001;
constexpr double AttackMs = 0.5 * 0.001;
void Ducker::SetRelease(double release) {
  m_Release = release;
  double realRelease = (release * MaxReleaseMs) + MinReleaseMs;
  m_ReleaseRate = exp(-1.0 / (realRelease * m_SampleRate));
}
void Ducker::SetSampleRate(double sampleRate) {
  m_SampleRate = sampleRate;
  SetRelease(m_Release);
  m_AttackRate = exp(-1.0 / (AttackMs * m_SampleRate));
}
void Ducker::SetDucking(double percent) {
  m_DuckingRatio = percent * percent;
  m_Ratio = (m_DuckingRatio * 13) + 1.0;
}
} // namespace VSTZ