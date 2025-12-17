#pragma once

#include "../Constraint.h"

struct SpringConstraint : public PairConstraint {
  SpringConstraint(Body* b0, Body* b1, double length = -1.0, double k = 15.0, double damping = 1.0);

  double length;
  double k;
  double damping;

  void solve() override;
  void draw() override;
};