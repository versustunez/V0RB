#pragma once

#include "Core/Instance.h"
#include "Editor/Ducking.h"
#include "Editor/Logo.h"

#include <GUI/Components/Components.h>
#include <TypeDefs.h>

namespace VSTZ {

class UI : public GUI::VComponent {
public:
  explicit UI(InstanceID id) : m_ID(id) { Init(); }
  void Init();
  void resized() override;
  void paint(juce::Graphics &g) override;

protected:
  InstanceID m_ID{};
  Scope<GUI::Switch> m_Bypass;
  Scope<Editor::Logo> m_Logo;
  Scope<GUI::Knob> m_Wet;
  Scope<GUI::Knob> m_Dry;
  Scope<GUI::Knob> m_Decay;
  Scope<GUI::Knob> m_Size;
  Scope<GUI::Knob> m_Brightness;

  Scope<Ducking> m_Ducking;

  Core::Instance *m_Instance;
};
} // namespace VSTZ
