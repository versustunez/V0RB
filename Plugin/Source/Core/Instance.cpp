#include <Core/Instance.h>

namespace VSTZ::Core {
Mutex Instance::s_CreateInstanceGuard{};
std::vector<Instance *> Instance::m_instances = {};
Instance::~Instance() {
  handler = nullptr;
}
Instance *Instance::get(InstanceID m_id) {
  // This should never be a problem tbh!
  return m_instances[m_id];
}
Instance *Instance::create() {
  Guard lockGuard(s_CreateInstanceGuard);
  // if m_instances is not empty we search for an empty slot... this happens on
  // start-time so no problem here :D
  if (!m_instances.empty()) {
    for (size_t i = 0; i < m_instances.size(); i++) {
      if (m_instances[i] == nullptr) {
        auto *instance = new Instance(i);
        instance->init();
        m_instances[i] = instance;
        return instance;
      }
    }
  }
  m_instances.push_back(new Instance(m_instances.size()));
  auto *instance = m_instances[m_instances.size() - 1];
  instance->init();
  return instance;
}

void Instance::remove(InstanceID m_id) {
  if (m_instances[m_id] != nullptr) {
    delete m_instances[m_id];
    m_instances[m_id] = nullptr;
  }
  bool isEmpty = true;
  for (auto &m_instance : m_instances) {
    if (m_instance != nullptr) {
      isEmpty = false;
      break;
    }
  }
  if (isEmpty)
    m_instances.clear();
}

State::InstanceState *Instance::getState(InstanceID id) {
  return &m_instances[id]->state;
}

void Instance::init() {
  handler.Create(id);
  state.m_ID = id;
}

} // namespace VSTZ::Core