#pragma once

#include "../Constraint.h"

struct SpringConstraint : public PairConstraint {
  SpringConstraint(System* system, ID id, ID b0, ID b1, float k = 150.0, float damping = 1.0);

  float length;
  float k;
  float damping;

  inline static const float minDistance{1.0};

  void solve() override;
  void draw(Draw::Color color) const override;
};