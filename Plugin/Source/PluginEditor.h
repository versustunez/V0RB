#pragma once

#include "JuceHeader.h"
#include "PluginProcessor.h"
#include "TypeDefs.h"
#include "UI.h"

#include <string>

class VSTEditor : public juce::AudioProcessorEditor,
                  public juce::OpenGLRenderer,
                  public juce::AsyncUpdater {
public:
  explicit VSTEditor(VSTProcessor &p, std::string id);
  ~VSTEditor() override;

  void paint(juce::Graphics &) override;
  void resized() override;
  void setupGL(bool vsync);

  void newOpenGLContextCreated() override;
  void renderOpenGL() override;
  void openGLContextClosing() override;

  void TriggerRepaint() { triggerAsyncUpdate(); }
  void handleAsyncUpdate() override { repaint(); }

protected:
  std::string m_id;
  VSTZ::InstanceID m_instanceId;
  juce::OpenGLContext m_openGLContext;
  VSTZ::Scope<VSTZ::UI> m_UI;
};
