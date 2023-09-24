#include "UI.h"

#include <Core/Config.h>
#include <Definitions.h>
#include <JuceHeader.h>
#include <PluginEditor.h>

typedef VSTZ::Theme::Colors Colors;

VSTEditor::VSTEditor(VSTProcessor &p, std::string id)
    : AudioProcessorEditor(&p),
      m_id(std::move(id)),
      m_instanceId(p.instance->id) {
  auto &config = VSTZ::Core::Config::get();
  config.registerEditor(m_id, this);
  setResizable(false, false);
  setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  if (config.properties()->asBool("useOpenGL", true))
    setupGL(config.properties()->asBool("vsync", true));
  m_UI.Create(m_instanceId);
  addAndMakeVisible(*m_UI);
  m_UI->setBounds(0, 0, getWidth(), getHeight());
  VSTZ::Core::Instance::get(m_instanceId)->Editor = this;
}

void VSTEditor::paint(juce::Graphics &g) {
  auto *theme = VSTZ::Core::Config::get().theme().Get();
  g.fillAll(theme->getColor(Colors::bg));
}
VSTEditor::~VSTEditor() {
  VSTZ::Core::Config::get().removeEditor(m_id);
  m_openGLContext.detach();
  VSTZ::Core::Instance::get(m_instanceId)->Editor = nullptr;
}
void VSTEditor::resized() {}
void VSTEditor::setupGL(bool vsync) {
#ifdef _DEBUG
  return;
#endif
  m_openGLContext.setRenderer(this);
  if (!vsync && !m_openGLContext.setSwapInterval(0)) {}
  m_openGLContext.setContinuousRepainting(false);
  m_openGLContext.setComponentPaintingEnabled(true);

  m_openGLContext.attachTo(*this);
  m_openGLContext.makeActive();
}
void VSTEditor::newOpenGLContextCreated() {}
void VSTEditor::renderOpenGL() { m_UI->repaint(); }
void VSTEditor::openGLContextClosing() {}
