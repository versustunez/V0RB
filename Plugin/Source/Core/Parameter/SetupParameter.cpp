#include "Handler.h"

#include <FMT.h>

namespace VSTZ::Core {

void ParameterHandler::SetupParameter() {
  constexpr auto percentCallback = [](float value, float) -> juce::String {
    return juce::String(value * 100, 1) + "%";
  };
  constexpr auto dbCallback = [](float value, float) -> juce::String {
    return juce::String(value, 2) + "dB";
  };
  constexpr auto secondsCB = [](float value, float) -> juce::String {
    if (value < 1) {
      return juce::String(value * 1000, 1) + "ms";
    }
    return juce::String(value, 2) + "s";
  };
  constexpr auto valueCB = [](float value, float) -> juce::String {
    return {value, 2};
  };

  constexpr double maxBrightness = 4.0;
  constexpr auto brightnessCB = [](float value, float) -> juce::String {
    if (value == 0)
      return "Normal";
    return juce::String(((value * -1) / maxBrightness) * 100, 2) + "% Darker";
  };
  AddParameter({.Name = "wet",
                .View = "Wet",
                .Min = 0,
                .Max = 1.0,
                .Value = 0.4,
                .ValueToStringCallback = percentCallback});
  AddParameter({.Name = "dry",
                .View = "Dry",
                .Min = 0,
                .Max = 1.0,
                .Value = 1.0,
                .ValueToStringCallback = percentCallback});
  AddParameter({.Name = "decay",
                .View = "Decay",
                .Min = 0.01,
                .Max = 10.0,
                .Value = 1.0,
                .ValueToStringCallback = secondsCB});
  AddParameter({.Name = "size",
                .View = "Room",
                .Min = 20.0,
                .Max = 200.0,
                .Value = 50.0,
                .ValueToStringCallback = valueCB});
  AddParameter({.Name = "brightness",
                .View = "Brightness",
                .Min = -maxBrightness,
                .Max = 0,
                .Value = -0.5,
                .ValueToStringCallback = brightnessCB});


  // Ducking...
  AddParameter({
      .Name = "ducking.power",
      .View = "Ducking Power",
      .Min = 0,
      .Max = 1,
      .Value = 0,
      .ValueToStringCallback = percentCallback
  });
  AddParameter({
      .Name = "ducking.release",
      .View = "Ducking Release",
      .Min = 0,
      .Max = 1,
      .Value = 1,
      .ValueToStringCallback = percentCallback
  });
  AddParameter({
      .Name = "ducking.threshold",
      .View = "Ducking Ducking",
      .Min = -70,
      .Max = 0,
      .Value = 0,
      .ValueToStringCallback = dbCallback
  });
  AddBoolParameter("bypass", "Bypass", false);
}
} // namespace VSTZ::Core