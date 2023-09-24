#pragma once

#include "RingBuffer.h"
#include "VisualBuffer.h"

#include <vector>
namespace VSTZ {
// Ducker is a Compressor that reduces the Sidechain
class Ducker {
public:

  double CalculateReduction(double in);
  void SetThreshold(double threshold);
  void SetRelease(double release);
  void SetSampleRate(double sampleRate);
  void SetDucking(double percent);

  VisualBuffer* GainReductionBuffer;
protected:
  double m_Threshold{-20};
  double m_ThresholdGain{0};
  double m_AttackRate{0};
  double m_Release{0};
  double m_ReleaseRate{0};
  double m_Envelope{1.0};
  double m_SampleRate{0};
  double m_DuckingRatio{0};
  double m_Ratio{0}; // that's magic. the user can select how much he wants to duck the signal what means (0-100% that's 1:1 -> 1:20)
};
}