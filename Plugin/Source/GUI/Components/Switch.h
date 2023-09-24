#pragma once
#include "BaseComponent.h"

#include <memory>
typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
namespace VSTZ::GUI {
class Switch : public BaseComponent {
public:
  Switch(const std::string &name, const std::string &showName, InstanceID id);
  ~Switch() override;
  void setButtonText(const std::string &);
  void resized() override;

private:
  Scope<juce::ToggleButton> m_button;
  std::unique_ptr<ButtonAttachment> m_attachment;
};
} // namespace VSTZ::GUI