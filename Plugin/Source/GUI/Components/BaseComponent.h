#pragma once
#include "Core/Parameter/Parameter.h"
#include "VComponent.h"

#include <JuceHeader.h>
#include <TypeDefs.h>

namespace VSTZ::GUI {
enum LabelPosition { NO_LABEL = 0, BOTTOM, TOP, CENTER };
struct Position {
  int x, y, w, h;
};

class BaseComponent : public VComponent {
public:
  BaseComponent(std::string name, std::string showName, InstanceID id);
  ~BaseComponent() override = default;
  std::string &name();
  std::string &showName();
  InstanceID id() const;
  LabelPosition m_labelPosition = LabelPosition::BOTTOM;
  Position resize();

  void createLabel(const std::string &text, bool visible);
  void removeLabel();
  void setText(const std::string &text);
  void setLabelPosition(const std::string &pos);

  static int precision();

protected:
  std::string m_name;
  std::string m_showName;
  Scope<juce::Label> m_label;
  InstanceID m_id;
  std::string m_selectorId;
  bool m_afterParsingCalled = false;
  Core::Parameter *m_parameter{nullptr};
};
} // namespace VSTZ::GUI