#pragma once

#include "fizz/System.h"

class Pendulum : public System {
 public:
  Pendulum(int nLinks, const DVec2& anchor, double distance, bool springs = false);
};