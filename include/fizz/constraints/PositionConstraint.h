#pragma once

#include "../Constraint.h"
#include "../Vec2.h"

struct PositionConstraint : public RangeConstraint {
  template <typename... Bs>
    requires(std::same_as<Bs, ID> && ...)
  PositionConstraint(System* system, ID id, const Vec2& _minPos, const Vec2& _maxPos, float _bounce, Bs... bodyIds)
      : RangeConstraint(system, id, bodyIds...), minPos(_minPos), maxPos(_maxPos), bounce(_bounce)
  {
  }

  Vec2 minPos, maxPos;
  float bounce;

  void solve() override;
};