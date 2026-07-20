#pragma once

#include "../Constraint.h"

struct DistanceConstraint : public PairConstraint {
  DistanceConstraint(System* system, ID id, ID b0, ID b1);

  float distance;

  void solve() override;
  void draw(Draw::Color color) const override;
};