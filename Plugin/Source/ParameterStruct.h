#pragma once

#include "Core/Parameter/Parameter.h"
#include "Filter.h"

namespace VSTZ {
struct Parameters {
  Core::Parameter *Bypass{nullptr};
  Core::Parameter *Wet{nullptr};
  Core::Parameter *Dry{nullptr};
  Core::Parameter *Decay{nullptr};
  Core::Parameter *RoomSize{nullptr};
  Core::Parameter *Brightness{nullptr};

  // Ducking
  Core::Parameter *DuckingThreshold{nullptr};
  Core::Parameter *DuckingRelease{nullptr};
  Core::Parameter *Ducking{nullptr};
};
} // namespace VSTZ
