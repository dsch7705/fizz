#include "fizz/constraints/PositionConstraint.h"

#include "fizz/Body.h"
#include "fizz/System.h"

void PositionConstraint::solve()
{
  for (ID b : m_bodies) {
    Body& body = m_system->getBody(b);

    DVec2& p0 = body.m_pos;
    DVec2& p0_last = body.m_lastPos;
    DVec2 Dp0 = p0 - p0_last;
    Dp0.scale(bounce);

    if (p0.x - body.radius < minPos.x) {
      p0.x = minPos.x + body.radius;
      p0_last.x = p0.x + Dp0.x;
    }
    else if (p0.x + body.radius > maxPos.x) {
      p0.x = maxPos.x - body.radius;
      p0_last.x = p0.x + Dp0.x;
    }

    if (p0.y - body.radius < minPos.y) {
      p0.y = minPos.y + body.radius;
      p0_last.y = p0.y + Dp0.y;
    }
    else if (p0.y + body.radius > maxPos.y) {
      p0.y = maxPos.y - body.radius;
      p0_last.y = p0.y + Dp0.y;
    }
  }
}