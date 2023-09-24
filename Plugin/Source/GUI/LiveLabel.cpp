#include "LiveLabel.h"

#include <VUtils/StringUtils.h>
#include <JuceHeader.h>

namespace VSTZ::GUIEvents {
LiveLabel::LiveLabel(GUI::Knob *knob, bool isSemi, std::string showName)
    : m_knob(knob),
      m_semiMode(isSemi),
      m_showName(std::move(showName)) {
  m_knob->slider()->addListener(this);
}
void LiveLabel::timerCallback() {
  if (m_sliderDragging)
    return;
  ticks++;
  if (ticks < 5)
    return;
  m_knob->setText(m_showName);
  stopTimer();
  timerStarted = false;
}
void LiveLabel::sliderValueChanged(juce::Slider *) {
  if (m_semiMode && !timerStarted) {
    startTimer(100);
    timerStarted = true;
  }
  ticks = 0;
  auto *parameter = m_knob->GetParameter();
  if (parameter) {
    m_knob->setText(
        m_knob->GetParameter()
            ->getText(parameter->convertTo0to1(m_knob->getValue()), 2)
            .toStdString());
  } else {
    m_knob->setText(VUtils::StringUtils::toString(m_knob->slider()->getValue(),
                                                  m_knob->precision(), true));
  }
}
void LiveLabel::stopLive() {
  ticks = 0;
  stopTimer();
  timerStarted = false;
}
LiveLabel::~LiveLabel() { m_knob->slider()->removeListener(this); }
void LiveLabel::sliderDragStarted(juce::Slider *) { m_sliderDragging = true; }
void LiveLabel::sliderDragEnded(juce::Slider *) { m_sliderDragging = false; }
} // namespace VSTZ::GUIEvents