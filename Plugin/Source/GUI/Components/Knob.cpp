#include <VUtils/StringUtils.h>
#include <Core/Config.h>
#include <Core/Instance.h>
#include <GUI/Components/Knob.h>
#include <GUI/LiveLabel.h>
#include <utility>

namespace VSTZ::GUI {
Knob::Knob(std::string name, std::string showName, InstanceID id)
    : BaseComponent(std::move(name), std::move(showName), id) {
  m_labelPosition = LabelPosition::BOTTOM;
  m_slider.Create();
  m_slider->setSliderStyle(getSliderStyle());
  m_slider->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
  m_slider->setComponentID(std::to_string(id));
  m_slider->setScrollWheelEnabled(true);
  m_slider->setPopupDisplayEnabled(false, true, getParentComponent(), 500);

  createLabel(m_showName, false);
  m_label->setEditable(true);
  m_label->addListener(this);
  m_label->setJustificationType(juce::Justification::centred);

  // get instance and try to add Slider Attachments
  auto *instance = Core::Instance::get(m_id);
  if (instance && instance->treeState) {
    auto state = instance->treeState;
    m_Parameter = state->getParameter(m_name);
    if (m_Parameter != nullptr) {
      m_attachment = std::make_unique<SliderAttachment>(*state, m_name, *m_slider);
    }
  }
  addAndMakeVisible(*m_slider);
  addAndMakeVisible(*m_label);
}
void Knob::paint(juce::Graphics &) {}
void Knob::resized() {
  auto position = resize();
  m_slider->setBounds(position.x, position.y, position.w, position.h);
}

void Knob::labelTextChanged(juce::Label *) {
  // We Ignore this but we have to implement this!
}
void Knob::editorShown(juce::Label *, juce::TextEditor &editor) {
  editor.setJustification(juce::Justification::centred);
  m_slider->getRange();
  editor.setText(
      VUtils::StringUtils::toString(m_slider->getValue(), precision(), true),
      false);
  if (m_liveLabel)
    m_liveLabel->stopLive();
}
void Knob::editorHidden(juce::Label *label, juce::TextEditor &) {
  auto text = label->getText().toStdString();
  double newValue = VUtils::StringUtils::toNumber(text, m_slider->getValue());
  m_slider->setValue(newValue);
  setText(m_showName);
}
Knob::~Knob() {
  m_label->removeListener(this);
}
void Knob::enableLiveLabel(bool isSemi) {
  m_liveLabel.Create(this, isSemi, showName());
  m_slider->setPopupDisplayEnabled(false, false, getParentComponent(), 0);
  if (!isSemi)
    m_liveLabel->sliderValueChanged(m_slider.Get());
}
juce::Slider *Knob::slider() { return m_slider.Get(); }
juce::Label *Knob::label() { return m_label.Get(); }
juce::Slider::SliderStyle Knob::getSliderStyle() {
  // look for user selection in config
  auto *props = Core::Config::get().properties();
  int val = props->asInt("sliderMode", -1);
  switch (val) {
  case 0: return juce::Slider::SliderStyle::Rotary;
  case 1: return juce::Slider::SliderStyle::RotaryHorizontalDrag;
  case 2: return juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag;
  default: return juce::Slider::SliderStyle::RotaryVerticalDrag;
  }
}
void Knob::setTooltip(bool tooltip) {
  m_slider->setPopupDisplayEnabled(false, tooltip, getParentComponent(), 500);
}
} // namespace VSTZ::GUI
