#pragma once

#include "BaseComponent.h"

#include <JuceHeader.h>
#include <memory>

// Forwards
namespace VSTZ::GUIEvents {
class LiveLabel;
}
namespace VSTZ::GUI {
class Knob : public BaseComponent, public juce::Label::Listener {
public:
  Knob(std::string name, std::string showName, InstanceID id);
  ~Knob() override;
  // Component
  void paint(juce::Graphics &g) override;
  void resized() override;

  // Juce Label Listener
  void labelTextChanged(juce::Label *labelThatHasChanged) override;
  void editorShown(juce::Label *label, juce::TextEditor &editor) override;
  void editorHidden(juce::Label *label, juce::TextEditor &editor) override;
  void enableLiveLabel(bool isSemi);
  void EnableLinearModeVertical() { m_slider->setSliderStyle(juce::Slider::SliderStyle::LinearVertical); }
  void EnableLinearModeHorizontal() { m_slider->setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal); }
  void EnableLinearModeBarVertical() { m_slider->setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical); }
  void EnableLinearModeBar() { m_slider->setSliderStyle(juce::Slider::SliderStyle::LinearBar); }

  juce::Slider *slider();
  juce::Label *label();

  static juce::Slider::SliderStyle getSliderStyle();

  void setMinMax(double min, double max, double interval = 0) {
    m_slider->setRange(min, max, interval);
  }
  void setValue(double value) { m_slider->setValue(value); }
  void setDefaultValue(double defaultValue) {
    m_slider->setDoubleClickReturnValue(true, defaultValue);
  }
  double getValue() { return m_slider->getValue(); }
  void setTooltip(bool tooltip);

  juce::RangedAudioParameter* GetParameter() { return m_Parameter; }

protected:
  Scope<juce::Slider> m_slider;
  std::unique_ptr<SliderAttachment> m_attachment;
  juce::RangedAudioParameter* m_Parameter{nullptr};
  Scope<GUIEvents::LiveLabel> m_liveLabel{nullptr};
};
} // namespace VSTZ::GUI