#pragma once

#include <TypeDefs.h>

#include <cstdlib>
namespace VSTZ::State {
class InstanceState {
public:
  explicit InstanceState(InstanceID id);
  InstanceID m_ID;
};
} // namespace VSTZ::State