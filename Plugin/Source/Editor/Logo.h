#pragma once

#include "GUI/Components/VComponent.h"

#include <TypeDefs.h>

namespace VSTZ::Editor {
class Logo : public GUI::VComponent {
public:
  explicit Logo(InstanceID id);
  void paint(juce::Graphics &g) override;
  void resized() override;

protected:
  InstanceID m_ID;
  std::unique_ptr<juce::Drawable> m_Image;
};
} // namespace Editor