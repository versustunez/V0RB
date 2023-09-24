#include <Core/Config.h>
#include <PluginEditor.h>

namespace VSTZ::Core {
Mutex Config::createGuard{};
Config::Config() {
  using VUtils::FileHandler;
  initConfig();
  VUtils::FileHandler::m_currentDir =
      m_config->file().getParentDirectory().getFullPathName().toStdString();
  m_theme.Create(m_config);
  m_theme->init();
}
Config::~Config() {
  m_config->save();
}
void Config::initConfig() {
  Guard lockGuard(m_guard);
  m_config = Properties::create("editor-config", "VeClip");
  juce::Desktop::getInstance().setGlobalScaleFactor(1.5);
}
void Config::registerEditor(std::string &id, VSTEditor *editor) {
  initLayout();
  initTheme();
  m_editors[id] = editor;
}
void Config::removeEditor(std::string &id) {
  m_editors.erase(id);
  if (m_editors.empty()) {
    look = nullptr;
    m_isThemeInit = false;
  }
}
Config &Config::get() {
  Guard lockGuard(createGuard);
  static Config instance;
  return instance;
}
void Config::registerProcessor(std::string &id,
                               juce::AudioProcessor *processor) {
  m_processors[id] = processor;
}
void Config::removeProcessor(std::string &id) {
  m_processors.erase(id);
  // If all Processors are gone remove VeNo! this will also free-up memory from
  // WaveTables and other stuff! :P
  if (m_processors.empty()) {
    Guard lockGuard(m_guard);
    m_config->save();
    sampleRate = -1.0;
  }
}
Properties *Config::properties() { return m_config.Get(); }

void Config::initLayout() {
  Guard lockGuard(m_guard);
  if (m_isLayoutInit)
    return;
  m_isLayoutInit = true;
  std::string file = m_config->asString("gui.main", "Bin::MainGui");
}
Ref<Theme::Theme> Config::theme() { return m_theme; }
void Config::initTheme() {
  Guard lockGuard(m_guard);
  if (m_isThemeInit)
    return;
  m_isThemeInit = true;
  look.Create();
}
} // namespace VSTZ::Core
