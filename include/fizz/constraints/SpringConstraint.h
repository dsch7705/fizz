#pragma once

#include "../Constraint.h"
#include "../DVec.h"

struct SpringConstraint : public PairConstraint {
  SpringConstraint(Body* b0, Body* b1, double length = -1.0, double k = 15.0, double damping = 1.0);

  double length;
  double k;
  double damping;

  void solve() override;
  void draw() override;

 private:
  // Difference in position between b0 and b1, normalized
  DVec2 m_n;
};