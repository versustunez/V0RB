#pragma once
#include "Core/Parameter/Parameter.h"

#include <string>

namespace VSTZ::Events {

class Event {
public:
  virtual ~Event() = default;
  virtual void Handle() {}
  template <class AsClass> AsClass *As() {
    return dynamic_cast<AsClass *>(this);
  }
  template <class AsClass> bool Is() {
    return dynamic_cast<AsClass *>(this) != nullptr;
  }
};

class ParameterChange: public Event {
public:
  void Handle() override {}
  Core::Parameter* Parameter{};
  double Value{0.0};
};

class Handler {
public:
  virtual ~Handler() = default;
  virtual void Handle(Event *event) = 0;
};

} // namespace VeNo::Events