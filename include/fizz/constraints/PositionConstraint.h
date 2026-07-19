#pragma once

#include "../Constraint.h"
#include "../DVec.h"

struct PositionConstraint : public RangeConstraint {
  template <typename... Bs>
    requires(std::same_as<Bs, ID> && ...)
  PositionConstraint(System* system, ID id, const DVec2& _minPos, const DVec2& _maxPos, double _bounce, Bs... bodyIds)
      : RangeConstraint(system, id, bodyIds...), minPos(_minPos), maxPos(_maxPos), bounce(_bounce)
  {
  }

  DVec2 minPos, maxPos;
  double bounce;

  void solve() override;
};