#pragma once

#include "Events/EventHandler.h"
#include "ParameterStruct.h"
#include "PluginProcessor.h"

#include <Core/Parameter/Handler.h>
#include <Core/State/InstanceState.h>
#include <JuceHeader.h>
#include <TypeDefs.h>
#include "Audio/RingBuffer.h"

namespace VSTZ::Core {
// Instance has a GetInstance method
class Instance {
public:
  InstanceID id;
  double bpm{120};

public:
  static Instance *get(InstanceID m_id);
  static Instance *create();
  static void remove(InstanceID m_id);
  static State::InstanceState *getState(InstanceID id);
  void init();
  Scope<ParameterHandler> handler{nullptr};
  juce::AudioProcessorValueTreeState *treeState = nullptr;
  State::InstanceState state{id};

  Events::EventHandler EventHandler{id};
  juce::AudioProcessorEditor *Editor{nullptr};
  VSTProcessor* Processor{nullptr};
  VSTZ::VisualBuffer InputValues;
  VSTZ::VisualBuffer GainReductionValues;

protected:
  explicit Instance(InstanceID _id) : id(_id) {}
  ~Instance();
  static std::vector<Instance *> m_instances;

protected:
  static Mutex s_CreateInstanceGuard;
};
} // namespace VSTZ::Core