#include "Core/Instance.h"
#include "Events/Event.h"

#include <Core/Parameter/Parameter.h>
#include <utility>

namespace VSTZ::Core {
Parameter::Parameter(std::string name, std::string showName, double min,
                     double max, double value, size_t id)
    : m_id(id),
      m_name(std::move(name)),
      m_showName(std::move(showName)),
      m_max(max),
      m_min(min),
      m_value(value) {}

bool Parameter::getBool() const { return m_value > 0.5; }
int Parameter::getInt() const { return (int)m_value; }
double Parameter::getValue() const { return m_value; }
void Parameter::setValue(double value) { m_value = value; }
std::unique_ptr<juce::RangedAudioParameter> Parameter::createParameter(
    ParameterTypes type, std::function<juce::String(float, float)> &function) {
  switch (type) {
  case Integer: {
    juce::AudioParameterIntAttributes attributes;
    if (function) {
      attributes = juce::AudioParameterIntAttributes()
                       .withStringFromValueFunction(function);
    }

    return std::make_unique<juce::AudioParameterInt>(
        m_name, m_showName, m_min, m_max, m_value, attributes);
  }
  case Float: {
    juce::AudioParameterFloatAttributes attributes;
    if (function) {
      attributes =
          juce::AudioParameterFloatAttributes().withStringFromValueFunction(
              function);
    }
    return std::make_unique<juce::AudioParameterFloat>(
        m_name, m_showName, juce::NormalisableRange<float>(m_min, m_max),
        m_value, attributes);
  }
  case Boolean:
    return std::make_unique<juce::AudioParameterBool>(m_name, m_showName,
                                                      m_value == 1);
  default: {
    juce::AudioParameterFloatAttributes attributes;
    if (function) {
      attributes =
          juce::AudioParameterFloatAttributes().withStringFromValueFunction(
              function);
    }
    return std::make_unique<juce::AudioParameterFloat>(
        m_name, m_showName, juce::NormalisableRange<float>(m_min, m_max),
        m_value, attributes);
  }
  }
}
double Parameter::getMin() const { return m_min; }
double Parameter::getMax() const { return m_max; }
std::string &Parameter::getShowName() { return m_showName; }
std::string &Parameter::getName() { return m_name; }
void Parameter::parameterValueChanged(int, float newValue) {
  setValue(m_min + newValue * (m_max - m_min));
  for (auto &fnc : m_CallbackFunctions) {
    fnc(newValue);
  }
  // Instance::get(m_id)->EventHandler.TriggerEvent(m_name, event);
}
// can be used for displays or something :D
void Parameter::parameterGestureChanged(int, bool) {}
void Parameter::SetValueAndNotifyHost(float value) {
  if (value != m_value) {
    m_value = value;
    m_InternalParameter->setValueNotifyingHost(
        m_InternalParameter->convertTo0to1(value));
  }
}
void Parameter::ResetToDefault() {
  SetValueAndNotifyHost(m_InternalParameter->getDefaultValue());
}
} // namespace VSTZ::Core