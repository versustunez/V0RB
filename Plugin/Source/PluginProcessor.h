#pragma once

#include "Audio/Ducker.h"
#include "Audio/Reverb.h"
#include "Events/EventHandler.h"
#include "JuceHeader.h"
#include "ParameterStruct.h"

#include <Core/Parameter/Parameter.h>

namespace VSTZ::Core {
class Instance;
}
class VSTProcessor : public juce::AudioProcessor {
public:
  VSTProcessor();
  ~VSTProcessor() override;

  void prepareToPlay(double, int) override;
  void releaseResources() override {}

  bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

  void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;
  void processBlock(juce::AudioBuffer<double> &, juce::MidiBuffer &) override;

  juce::AudioProcessorEditor *createEditor() override;
  bool hasEditor() const override { return true; }

  const juce::String getName() const override { return JucePlugin_Name; }

  bool acceptsMidi() const override { return true; }
  bool producesMidi() const override { return false; }
  bool isMidiEffect() const override { return false; }
  double getTailLengthSeconds() const override { return 0.0; }

  int getNumPrograms() override { return 1; }
  int getCurrentProgram() override { return 0; }
  bool supportsDoublePrecisionProcessing() const override;
  void setCurrentProgram(int) override {}
  const juce::String getProgramName(int) override {
    return juce::String(JucePlugin_Name);
  }
  void changeProgramName(int, const juce::String &) override {}

  void getStateInformation(juce::MemoryBlock &destData) override;
  void setStateInformation(const void *data, int sizeInBytes) override;
public:
  VSTZ::Parameters& GetParameters() { return m_Parameters; }
public:
  VSTZ::Core::Instance *instance = nullptr;
  VSTZ::Reverb<8,4> m_Reverb{};
  VSTZ::Ducker m_Ducker{};

private:
  std::string m_Id;
  juce::AudioProcessorValueTreeState m_TreeState;
  VSTZ::Parameters m_Parameters{};
};
