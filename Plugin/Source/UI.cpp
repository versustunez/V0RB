#include "UI.h"

#include "Core/Config.h"

#include <FMT.h>

namespace VSTZ {

constexpr static int TabHeight = 40;

void UI::Init() {
  m_Instance = Core::Instance::get(m_ID);
  m_Bypass.Create("bypass", "Bypass", m_ID);
  m_Bypass->setButtonText("Bypass");

  m_Wet.Create("wet", "Wet", m_ID);
  m_Wet->enableLiveLabel(true);
  m_Dry.Create("dry", "Dry", m_ID);
  m_Dry->enableLiveLabel(true);

  m_Decay.Create("decay", "Decay", m_ID);
  m_Decay->EnableLinearModeBarVertical();
  m_Decay->setTooltip(false);

  m_Size.Create("size", "Size", m_ID);
  m_Size->EnableLinearModeBarVertical();
  m_Size->setTooltip(false);

  m_Brightness.Create("brightness", "Brightness", m_ID);
  m_Brightness->EnableLinearModeBarVertical();
  m_Brightness->setTooltip(false);

  m_Ducking.Create("", "", m_ID);

  m_Logo.Create(m_ID);
  addAndMakeVisible(*m_Logo);
  addAndMakeVisible(*m_Bypass);
  addAndMakeVisible(*m_Wet);
  addAndMakeVisible(*m_Dry);
  addAndMakeVisible(*m_Decay);
  addAndMakeVisible(*m_Size);
  addAndMakeVisible(*m_Brightness);

  addAndMakeVisible(*m_Ducking);
}
void UI::resized() {
  m_Bypass->setBounds(getWidth() - 70, 0, 70, 40);
  m_Logo->setBounds(5, 5, 100, 30);
  int specHeight = getHeight() - TabHeight;
  float onePercent = getWidth() * 0.01f;
  int xWidth = onePercent * 30;
  m_Ducking->setBounds(0, TabHeight, xWidth, specHeight);
  juce::Rectangle<int> newBoundsCenter{(int)onePercent * 33, TabHeight + 10,
                                       (int)onePercent * 44, specHeight - 20};
  juce::Grid grid;
  using Track = juce::Grid::TrackInfo;
  using Fr = juce::Grid::Fr;

  grid.templateRows = {Track(Fr(1)), Track(Fr(1)), Track(Fr(1))};
  grid.templateColumns = {Track(Fr(1))};
  grid.rowGap = juce::Grid::Px(10);

  grid.items = {
      juce::GridItem(*m_Decay),
      juce::GridItem(*m_Size),
      juce::GridItem(*m_Brightness),
  };
  grid.performLayout(newBoundsCenter);

  grid.templateRows = {Track(Fr(1)), Track(Fr(1))};
  grid.items = {
      juce::GridItem(*m_Dry),
      juce::GridItem(*m_Wet),
  };
  newBoundsCenter = {(int)onePercent * 80, TabHeight + 10, (int)onePercent * 20,
                     specHeight - 20};
  grid.performLayout(newBoundsCenter);
}

void UI::paint(juce::Graphics &g) {
  auto theme = Core::Config::get().theme();
  auto bg = theme->getColor(Theme::Colors::root);
  g.fillAll(bg);

  g.setColour(theme->getColor(Theme::Colors::accent).withAlpha(.1f));
  g.fillRect(0, 0, getWidth(), TabHeight);

  int specHeight = getHeight() - TabHeight;
  float onePercent = getWidth() * 0.01f;
  juce::Rectangle<int> newBounds = {(int)onePercent * 80, TabHeight,
                                    (int)onePercent * 20, specHeight};
  bg = theme->getColor(Theme::Colors::bg);
  g.setColour(bg);
  g.fillRect(newBounds);
}
} // namespace VSTZ
