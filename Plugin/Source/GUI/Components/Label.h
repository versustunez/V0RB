#pragma once
#include "BaseComponent.h"

#include <JuceHeader.h>

namespace VSTZ::GUI {
class Label : public BaseComponent {
public:
  Label(std::string name, std::string showName, InstanceID id);
  void setText(std::string text);
  void setAlign(const std::string &align);
  void resized() override;
  juce::Label* label() { return m_label.Get(); }

protected:
  std::string m_text;
  Scope<juce::Label> m_label;
};
} // namespace VSTZ::GUI