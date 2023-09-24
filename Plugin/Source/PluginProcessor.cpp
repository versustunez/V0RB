#include "PluginProcessor.h"

#include "Core/Config.h"
#include "Core/Instance.h"
#include "PluginEditor.h"

#include <FMT.h>
#include <algorithm>

VSTProcessor::VSTProcessor()
    : AudioProcessor(
          BusesProperties()
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      instance(VSTZ::Core::Instance::create()),
      m_Id(juce::Uuid().toString().toStdString()),
      m_TreeState(*this, nullptr, JucePlugin_Name,
                  instance->handler->SetupProcessor()) {
  instance->treeState = &m_TreeState;
  instance->Processor = this;
  m_Ducker.GainReductionBuffer = &instance->GainReductionValues;
  VSTZ::Core::Config::get().registerProcessor(m_Id, this);
  auto &handler = instance->handler;
  m_Parameters.Bypass = handler->GetParameter("bypass");
  m_Reverb.Buffer = &instance->InputValues;
  {
    // Ducker
    m_Parameters.Ducking = handler->GetParameter("ducking.power");
    m_Parameters.DuckingRelease = handler->GetParameter("ducking.release");
    m_Parameters.DuckingThreshold = handler->GetParameter("ducking.threshold");

    m_Parameters.DuckingThreshold->RegisterChangeFunction([this](double value) {
      m_Ducker.SetThreshold(m_Parameters.DuckingThreshold->getValue());
    });
    m_Parameters.DuckingRelease->RegisterChangeFunction([this](double value) {
      m_Ducker.SetRelease(m_Parameters.DuckingRelease->getValue());
    });
    m_Parameters.Ducking->RegisterChangeFunction([this](double value) {
      m_Ducker.SetDucking(m_Parameters.Ducking->getValue());
    });
    // For the ducker! <3
    m_Ducker.SetThreshold(m_Parameters.DuckingThreshold->getValue());
    m_Ducker.SetRelease(m_Parameters.DuckingRelease->getValue());
    m_Ducker.SetDucking(m_Parameters.Ducking->getValue());
  }
  {
    m_Parameters.Dry = handler->GetParameter("dry");
    m_Parameters.Wet = handler->GetParameter("wet");
    m_Parameters.Decay = handler->GetParameter("decay");
    m_Parameters.RoomSize = handler->GetParameter("size");
    m_Parameters.Brightness = handler->GetParameter("brightness");
    m_Parameters.Decay->RegisterChangeFunction([this](double value) {
      m_Reverb.SetDelayTime(m_Parameters.Decay->getValue());
    });
    m_Parameters.RoomSize->RegisterChangeFunction([this](double value) {
      m_Reverb.SetRoomSize(m_Parameters.RoomSize->getValue());
    });
    m_Parameters.Brightness->RegisterChangeFunction([this](double value) {
      m_Reverb.SetBrightness(m_Parameters.Brightness->getValue());
    });

    m_Reverb.SetRoomSize(m_Parameters.RoomSize->getValue());
    m_Reverb.SetDelayTime(m_Parameters.Decay->getValue());
    m_Reverb.SetBrightness(m_Parameters.Brightness->getValue());
  }
}

template <typename T>
static void ProcessBlock(juce::AudioBuffer<T> &buffer,
                         VSTProcessor &processor) {
  juce::ScopedNoDenormals noDenormals;

  for (int i = processor.getTotalNumInputChannels();
       i < processor.getTotalNumOutputChannels(); ++i)
    buffer.clear(i, 0, buffer.getNumSamples());

  auto &parameters = processor.GetParameters();
  if (parameters.Bypass->getBool())
    return;

  auto &verb = processor.m_Reverb;
  verb.SetDry(parameters.Dry->getValue());
  verb.SetWet(parameters.Wet->getValue());
  bool hasRightChannel = buffer.getNumChannels() > 1;
  auto *leftBuffer = buffer.getWritePointer(0);
  auto *rightBuffer = leftBuffer;
  if (hasRightChannel) {
    rightBuffer = buffer.getWritePointer(1);
  }
  verb.Configure(verb.GetSampleRate());
  verb.ProcessStereo((float *)leftBuffer, (float *)rightBuffer,
                     buffer.getNumSamples(), processor.m_Ducker);
}

void VSTProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                juce::MidiBuffer &) {
  ProcessBlock(buffer, *this);
}

juce::AudioProcessorEditor *VSTProcessor::createEditor() {
  return new VSTEditor(*this, m_Id);
}

void VSTProcessor::getStateInformation(juce::MemoryBlock &destData) {
  auto state = m_TreeState.copyState();
  auto xmlState = VSTZ::Scope<juce::XmlElement>();
  xmlState.Create("State");
  xmlState->addChildElement(state.createXml().release());
  copyXmlToBinary(*xmlState, destData);
}

void VSTProcessor::setStateInformation(const void *data, int sizeInBytes) {
  std::unique_ptr<juce::XmlElement> xmlState(
      getXmlFromBinary(data, sizeInBytes));
  if (xmlState == nullptr)
    return;
  auto *parameters = xmlState->getChildByName(m_TreeState.state.getType());
  auto name = xmlState->getStringAttribute("name");
  if (parameters)
    m_TreeState.replaceState(juce::ValueTree::fromXml(*parameters));
}

VSTProcessor::~VSTProcessor() {
  VSTZ::Core::Config::get().removeProcessor(m_Id);
  VSTZ::Core::Instance::remove(instance->id);
}

void VSTProcessor::prepareToPlay(double sampleRate, int) {
  auto &config = VSTZ::Core::Config::get();
  // IT'S OKAY BECAUSE THEY ARE LIKELY TO NOT CHANGE OR BE INTEGERS AT ALL.
  if (config.sampleRate != sampleRate) {
    config.sampleRate = sampleRate;
  }
  m_Reverb.Configure(sampleRate);
  m_Reverb.SetBrightness(m_Parameters.Brightness->getValue());
  m_Ducker.SetSampleRate(sampleRate);
}
bool VSTProcessor::isBusesLayoutSupported(
    const juce::AudioProcessor::BusesLayout &layouts) const {
  if (layouts.getMainInputChannelSet() == juce::AudioChannelSet::disabled() ||
      layouts.getMainOutputChannelSet() == juce::AudioChannelSet::disabled())
    return false;

  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    return false;

  return layouts.getMainInputChannelSet() == layouts.getMainOutputChannelSet();
}
bool VSTProcessor::supportsDoublePrecisionProcessing() const { return false; }
void VSTProcessor::processBlock(juce::AudioBuffer<double> &buffer,
                                juce::MidiBuffer &) {
  ProcessBlock(buffer, *this);
}

juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new VSTProcessor();
}
