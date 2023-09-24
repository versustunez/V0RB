#pragma once

#include "Audio/RingBuffer.h"
#include "Audio/VisualBuffer.h"
#include "GUI/Components/BaseComponent.h"
#include "GUI/Components/Knob.h"

namespace VSTZ {

class Threshold : public GUI::VComponent, public juce::Timer {
public:
  Threshold(Core::Parameter* param, VisualBuffer* buffer, VisualBuffer* reductionBuffer);
  void paint(juce::Graphics &g) override;
  void resized() override;
  void timerCallback() override;
  void UpdatePath();
  void mouseDrag(const juce::MouseEvent &event) override;
  void mouseUp(const juce::MouseEvent &event) override;

protected:
  Core::Parameter* m_ThresholdParameter;
  VisualBuffer* m_InstanceBuffer{nullptr};
  VisualBuffer* m_GainReductionBuffer{nullptr};
  juce::Path m_Path;
  juce::Path m_GRPath;
  double m_LastPosY{0};
};

class Ducking : public GUI::BaseComponent {
public:
  Ducking(std::string name, std::string showName, InstanceID id);
  void resized() override;
  void paint(juce::Graphics& g) override;
protected:

  Scope<Threshold> m_Threshold;
  Scope<GUI::Knob> m_Release;
  Scope<GUI::Knob> m_Amount;

};

} // namespace VSTZ