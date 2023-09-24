#include "EventHandler.h"

namespace VSTZ::Events {
std::string EventHandler::PARAMETER_CHANGE_KEY = "parameter-changed";
void EventHandler::AddEvent(std::string &name, Event *event) {
  m_events[name] = event;
}
void EventHandler::RemoveEvent(std::string &name) { m_events.erase(name); }
void EventHandler::Handle(std::string &name) {
  if (m_events.find(name) != m_events.end())
    m_events[name]->Handle();
}
EventHandler::EventHandler(InstanceID id) : m_id(id) {}
EventHandler::~EventHandler() {
  cancelPendingUpdate();
  for (auto *owningHandler : m_owningHandlers) {
    delete owningHandler;
  }
  m_owningHandlers.clear();
}

void EventHandler::AddHandler(const std::string &name, Handler *handler) {
  juce::ScopedLock local_lock(m_handlerLock);
  auto &handlerList = m_handler[name];
  if (std::find(handlerList.begin(), handlerList.end(), handler) !=
      handlerList.end()) {
    return;
  }
  handlerList.push_back(handler);
}

void EventHandler::RemoveHandler(const std::string &name, Handler *handler) {
  juce::ScopedLock local_lock(m_handlerLock);
  auto handlerIt = m_handler.find(name);
  if (handlerIt == m_handler.end())
    return;
  auto &handlerList = handlerIt->second;
  auto it = std::find(handlerList.begin(), handlerList.end(), handler);
  if (it == handlerList.end())
    return;
  handlerList.erase(it);

  if (handlerList.empty()) {
    m_handler.erase(name);
  }
}

void EventHandler::TriggerEvent(const std::string &name, Event *event) {
  juce::ScopedLock local_lock(m_queueLock);
  m_queue.push(QueuedEvent{event, name});
  if (!isUpdatePending()) {
    triggerAsyncUpdate();
  }
}
void EventHandler::handleAsyncUpdate() {
  std::queue<QueuedEvent> queue;
  {
    juce::ScopedLock local_lock(m_queueLock);
    queue = std::move(m_queue);
    m_queue = {};
  }
  while (!queue.empty()) {
    juce::ScopedLock local_lock(m_handlerLock);
    auto &front = queue.front();
    if (m_handler.find(front.name) != m_handler.end()) {
      auto &handlerList = m_handler[front.name];
      for (auto *handler : handlerList)
        handler->Handle(front.event);
    }
    delete front.event;
    queue.pop();
  }
}
} // namespace VSTZ::Events