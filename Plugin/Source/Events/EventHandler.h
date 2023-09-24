#pragma once

#include "Event.h"

#include <JuceHeader.h>
#include <TypeDefs.h>
#include <queue>
#include <string>
#include <unordered_map>

namespace VSTZ::Events {
struct QueuedEvent {
  Events::Event *event{};
  std::string name{};
};
class EventHandler : juce::AsyncUpdater {
public:
  explicit EventHandler(InstanceID id);
  EventHandler(const EventHandler &) = delete;
  ~EventHandler() override;

  void AddEvent(std::string &name, Event *);
  void RemoveEvent(std::string &name);
  void TriggerEvent(const std::string &name, Event *event);

  void AddHandler(const std::string &name, Handler *handler);

  void RemoveHandler(const std::string &name, Handler *handler);

  void Handle(std::string &name);

public:
  static std::string PARAMETER_CHANGE_KEY;

private:
  void handleAsyncUpdate() override;

protected:
  InstanceID m_id;
  std::unordered_map<std::string, Event *> m_events;
  std::unordered_map<std::string, std::vector<Handler *>> m_handler;
  std::vector<Handler *> m_owningHandlers;
  std::queue<QueuedEvent> m_queue;
  juce::CriticalSection m_queueLock;
  juce::CriticalSection m_handlerLock;
};
} // namespace VSTZ::Events