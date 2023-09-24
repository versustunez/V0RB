#include <Core/Instance.h>
#include <GUI/Components/BaseComponent.h>

namespace VSTZ::GUI {
BaseComponent::BaseComponent(std::string name, std::string showName,
                             InstanceID id)
    : m_name(std::move(name)),
      m_showName(std::move(showName)),
      m_id(id) {

  auto *instance = Core::Instance::get(m_id);
  if (instance) {
    auto *parameter = instance->handler->GetParameter(m_name);
    if (parameter)
      m_parameter = parameter;
  }
}

std::string &BaseComponent::name() { return m_name; }
std::string &BaseComponent::showName() { return m_showName; }

InstanceID BaseComponent::id() const { return m_id; }
void BaseComponent::createLabel(const std::string &text, bool visible) {
  m_label = std::make_unique<juce::Label>();
  setText(text);
  if (visible)
    addAndMakeVisible(*m_label);
}

void BaseComponent::removeLabel() {
  removeChildComponent(m_label.Get());
  m_label.Reset();
}

void BaseComponent::setText(const std::string &text) {
  if (m_label)
    m_label->setText(text, juce::NotificationType::dontSendNotification);
}
Position BaseComponent::resize() {
  if (m_label) {
    const auto &font = m_label->getFont();
    m_label->setFont(font.withHeight(13.0f));
    int height = 18;
    int labelHeight = 16;
    int secHeight = getHeight() - height;
    LabelPosition position = m_labelPosition;
    if (position == LabelPosition::TOP) {
      m_label->setBounds(0, 0, getWidth(), labelHeight);
      return {0, height, getWidth(), secHeight};
    } else if (position == LabelPosition::BOTTOM) {
      m_label->setBounds(0, secHeight, getWidth(), labelHeight);
      return {0, 0, getWidth(), secHeight};
    } else if (position == LabelPosition::CENTER) {
      int halfHeight = (getHeight() - height) / 2;
      m_label->setBounds(0, halfHeight, getWidth(), labelHeight);
      return {0, 0, getWidth(), getHeight()};
    }
  }
  return {0, 0, getWidth(), getHeight()};
}
void BaseComponent::setLabelPosition(const std::string &label) {
  if (label == "no")
    m_labelPosition = LabelPosition::NO_LABEL;
  else if (label == "bottom")
    m_labelPosition = LabelPosition::BOTTOM;
  else if (label == "top")
    m_labelPosition = LabelPosition::TOP;
  else if (label == "center")
    m_labelPosition = LabelPosition::CENTER;
}

int BaseComponent::precision() { return 2; }

} // namespace VSTZ::GUI
