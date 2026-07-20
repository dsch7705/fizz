#pragma once

#include "fizz/System.h"

class Pendulum : public System {
 public:
  Pendulum(int nLinks, const Vec2& anchor, float distance, bool springs = false);
};