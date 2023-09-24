#include <Core/Parameter/Handler.h>
#include <Core/Parameter/Parameter.h>
#include <JuceHeader.h>

namespace VSTZ::Core {
ParameterHandler::ParameterHandler(InstanceID id) : m_ID(id) {}
ParameterHandler::~ParameterHandler() {
  for (auto &param : m_Parameters)
    delete param.second;
  m_Params.clear();
  m_ParamMaps.clear();
  m_Parameters.clear();
}

void ParameterHandler::AddParameter(const std::string &name,
                                    const std::string &showName, double min,
                                    double max, double value,
                                    ParameterTypes type) {
  AddParameter({
      .Name = name,
      .View = showName,
      .Min = min,
      .Max = max,
      .Value = value,
      .Type = type,
  });
}

void ParameterHandler::AddParameter(ParameterInfo info) {
  if (info.Value == -std::numeric_limits<double>::infinity())
    info.Value = info.Min;
  m_ParamMaps.push_back(info.Name);
  auto *parameter =
      new Parameter(info.Name, info.View, info.Min, info.Max, info.Value, m_ID);
  m_Parameters[info.Name] = parameter;
  m_Params.push_back(
      parameter->createParameter(info.Type, info.ValueToStringCallback));
  parameter->SetRangedParameter(m_Params.back().get());
  m_Params.back()->addListener(parameter);
}

void ParameterHandler::AddBoolParameter(const std::string &name,
                                        const std::string &showName,
                                        bool defaultValue) {
  AddParameter({
      .Name = name,
      .View = showName,
      .Min = 0.0,
      .Max = 1.0,
      .Value = defaultValue ? 1.0 : 0.0,
      .Type = ParameterTypes::Boolean,
  });
}

juce::AudioProcessorValueTreeState::ParameterLayout
ParameterHandler::SetupProcessor() {
  if (m_Params.empty())
    SetupParameter();
  return {m_Params.begin(), m_Params.end()};
}
void ParameterHandler::parameterChanged(const juce::String &, float) {}
std::string ParameterHandler::GetShowName(const std::string &parameter) {
  if (m_Parameters.find(parameter) != m_Parameters.end())
    return m_Parameters[parameter]->getShowName();
  return "Unknown";
}
Core::Parameter *ParameterHandler::GetParameter(const std::string &name) {
  if (m_Parameters.find(name) != m_Parameters.end())
    return m_Parameters[name];
  return nullptr;
}
Parameter *ParameterHandler::operator[](const char *name) {
  if (m_Parameters.find(name) != m_Parameters.end())
    return m_Parameters[name];
  return nullptr;
}

} // namespace VSTZ::Core
