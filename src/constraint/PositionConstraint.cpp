#include "fizz/constraint/PositionConstraint.h"

#include "fizz/core/Body.h"

void PositionConstraint::solve()
{
  for (Body* b0 : m_bodies) {
    assert(b0 != nullptr);

    DVec2& p0 = b0->m_pos;
    DVec2& p0_last = b0->m_lastPos;
    DVec2 Dp0 = p0 - p0_last;
    Dp0.scale(bounce);

    if (p0.x - b0->radius < minPos.x) {
      p0.x = minPos.x + b0->radius;
      p0_last.x = p0.x + Dp0.x;
    }
    else if (p0.x + b0->radius > maxPos.x) {
      p0.x = maxPos.x - b0->radius;
      p0_last.x = p0.x + Dp0.x;
    }

    if (p0.y - b0->radius < minPos.y) {
      p0.y = minPos.y + b0->radius;
      p0_last.y = p0.y + Dp0.y;
    }
    else if (p0.y + b0->radius > maxPos.y) {
      p0.y = maxPos.y - b0->radius;
      p0_last.y = p0.y + Dp0.y;
    }
  }
}