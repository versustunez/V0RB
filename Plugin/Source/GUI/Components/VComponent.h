#pragma once

#include <JuceHeader.h>

namespace VSTZ::GUI {

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

class VComponent : public juce::Component {
public:
  template <class ClassType> ClassType *Parent() {
    return findParentComponentOfClass<ClassType>();
  }

  template <class AsClass> AsClass *As() {
    return dynamic_cast<AsClass *>(this);
  }
};
} // namespace VSTZ::GUI