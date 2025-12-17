#pragma once

#include "../Constraint.h"

struct SpringConstraint : public PairConstraint {
  SpringConstraint(Body* b0, Body* b1, double k = 150.0, double damping = 1.0);

  double length;
  double k;
  double damping;

  inline static const double minDistance{1.0};

  void solve() override;
  void draw() override;
};