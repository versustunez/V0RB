#include "Ducking.h"

#include "Core/Config.h"
#include "Core/Instance.h"

#include <utility>

namespace VSTZ {
Threshold::Threshold(Core::Parameter *param, VisualBuffer *buffer,
                     VisualBuffer *reductionBuffer)
    : m_ThresholdParameter(param),
      m_InstanceBuffer(buffer),
      m_GainReductionBuffer(reductionBuffer) {
  startTimer(16 * 4);
}

void Threshold::paint(juce::Graphics &g) {
  g.setColour(juce::Colour(255, 255, 255).withAlpha(.2f));
  g.drawRect(getLocalBounds());
  g.setColour(juce::Colour(66, 245, 141).withAlpha(0.8f));
  g.strokePath(m_Path, juce::PathStrokeType(0.2f));
  g.setColour(juce::Colour(245, 136, 50).withAlpha(1.0f));
  g.strokePath(m_GRPath, juce::PathStrokeType(0.2f));

  // SET Threshold Slider
  double minValue = m_ThresholdParameter->getMin();
  double val = m_ThresholdParameter->getValue();
  double posY = juce::jmap(val, minValue, 0.0, (double)getHeight(), 0.0);

  g.setColour(juce::Colour(245, 167, 66));
  g.drawLine(0, (float)posY, (float)getWidth(), (float)posY, 1.5f);

  g.setFont(11);
  double textPosY = posY < (float)getHeight() / 2 ? posY + 1 : posY - 13;
  g.drawText(juce::String(m_ThresholdParameter->getValue(), 2), 0,
             (int)textPosY, getWidth(), 12, juce::Justification::centred);

  g.setFont(13);
}

void Threshold::timerCallback() {
  UpdatePath();
  repaint();
}

void Threshold::resized() { UpdatePath(); }

void Threshold::UpdatePath() {
  if (m_InstanceBuffer == nullptr || m_GainReductionBuffer == nullptr)
    return;

  m_Path.clear();
  m_Path.startNewSubPath(0, (float)getHeight());
  double x = 0;
  double xInc = (double)getWidth() / (VisualBuffer::Size - 1);
  double minValue = m_ThresholdParameter->getMin();
  for (auto value : m_InstanceBuffer->Buffer) {
    double posY = juce::jmap(juce::Decibels::gainToDecibels(value, minValue),
                             minValue, 0.0, 1.0, 0.0);
    m_Path.lineTo((float)x, (float)posY * (float)getHeight());
    x += xInc;
  }

  m_GRPath.clear();
  m_GRPath.startNewSubPath(0, (float)getHeight());
  x = 0;
  for (auto value : m_GainReductionBuffer->Buffer) {
    double posY = juce::jmap(juce::Decibels::gainToDecibels(value, minValue),
                             minValue, 0.0, 1.0, 0.0);
    m_GRPath.lineTo((float)x, (float)posY * (float)getHeight());
    x += xInc;
  }
}

void Threshold::mouseDrag(const juce::MouseEvent &event) {
  double minValue = m_ThresholdParameter->getMin();
  double maxValue = m_ThresholdParameter->getMax();
  double val = m_ThresholdParameter->getValue();
  double posY = juce::jmap(val, minValue, maxValue, (double)getHeight(), 0.0);
  posY -= (m_LastPosY - event.getDistanceFromDragStartY());
  posY = posY > (double)getHeight() ? (double)getHeight() : posY < 0 ? 0 : posY;
  double newValue =
      juce::jmap(posY, (double)getHeight(), 0.0, minValue, maxValue);
  m_ThresholdParameter->SetValueAndNotifyHost((float)newValue);
  m_LastPosY = event.getDistanceFromDragStartY();

  repaint();
}

void Threshold::mouseUp(const juce::MouseEvent &event) { m_LastPosY = 0; }

Ducking::Ducking(std::string name, std::string showName, InstanceID id)
    : BaseComponent(std::move(name), std::move(showName), id) {
  auto *instance = Core::Instance::get(id);
  auto threshold = instance->handler->GetParameter("ducking.threshold");
  m_Threshold.Create(threshold, &instance->InputValues, &instance->GainReductionValues);

  m_Release.Create("ducking.release", "Release", id);
  m_Release->EnableLinearModeBarVertical();
  m_Release->setTooltip(false);

  m_Amount.Create("ducking.power", "Ducking", id);
  m_Amount->EnableLinearModeBarVertical();
  m_Amount->setTooltip(false);

  addAndMakeVisible(*m_Threshold);
  addAndMakeVisible(*m_Release);
  addAndMakeVisible(*m_Amount);
}

void Ducking::resized() {
  constexpr float controlHeight = 56.0f; // because of linear buttons...
  constexpr int gap = 10;
  constexpr int dualGap = gap * 2;
  int maxWidth = getWidth() - dualGap;
  int maxHeight = getHeight() - dualGap;
  int thresholdHeight = maxHeight - int(controlHeight * 1.4);

  int startY = gap;
  m_Threshold->setBounds(gap, startY, maxWidth, thresholdHeight);
  startY += thresholdHeight + dualGap;

  int controlWidth = int((float)maxWidth * 0.5f) - gap;
  m_Release->setBounds(gap, startY, controlWidth, controlHeight);
  m_Amount->setBounds((gap * 3) + controlWidth, startY, controlWidth,
                      controlHeight);
}

void Ducking::paint(juce::Graphics &g) {
  auto bg = Core::Config::get().theme()->getColor(Theme::Colors::bg);
  g.fillAll(bg);
}
} // namespace VSTZ