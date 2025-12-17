#pragma once

#include "../Constraint.h"

struct DistanceConstraint : public PairConstraint {
  DistanceConstraint(Body* b0, Body* b1);

  double distance;

  void solve() override;
  void draw() override;
};