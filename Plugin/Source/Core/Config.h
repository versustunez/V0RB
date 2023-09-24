#pragma once

#include <Core/PropertiesWrapper.h>
#include <GUI/Theme/LookAndFeel/LookHandler.h>
#include <GUI/Theme/Theme.h>
#include <JuceHeader.h>
#include <TypeDefs.h>
#include <unordered_map>

class VSTEditor;
namespace VSTZ::Core {
struct Config {
  static Config &get();
  void registerEditor(std::string &id, VSTEditor *editor);
  void removeEditor(std::string &id);
  void registerProcessor(std::string &id, juce::AudioProcessor *processor);
  void removeProcessor(std::string &id);
  double sampleRate = -1;
  Properties *properties();

  Ref<Theme::Theme> theme();
  Scope<VSTZ::GUI::LookHandler> look{nullptr};

protected:
  Config();
  ~Config();
  void initConfig();
  Mutex m_guard;
  static Mutex createGuard;
  Ref<Properties> m_config{};
  Map<std::string, VSTEditor *> m_editors;
  Map<std::string, juce::AudioProcessor *> m_processors;
  bool m_isLayoutInit = false;
  bool m_isThemeInit = false;
  Ref<Theme::Theme> m_theme{};
  void initTheme();
  void initLayout();
};
} // namespace VSTZ::Core