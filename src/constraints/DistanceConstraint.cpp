#include "fizz/constraints/DistanceConstraint.h"

#include "fizz/Body.h"
#include "fizz/Draw.h"

DistanceConstraint::DistanceConstraint(Body* b0, Body* b1) : PairConstraint(b0, b1)
{
  distance = (m_b1->pos() - m_b0->pos()).mag();
}

void DistanceConstraint::solve()
{
  assert(m_b0 != nullptr && m_b1 != nullptr);

  DVec2& p0 = m_b0->m_pos;
  DVec2& p1 = m_b1->m_pos;
  DVec2 diff = p1 - p0;
  double mag = diff.mag();
  diff.normalize();

  double w0 = 1.0 / m_b0->mass;
  double w1 = 1.0 / m_b1->mass;
  double wSum = w0 + w1;

  double delta = (mag - distance) / wSum;

  if (m_b0->isKinematic) {
    p1 -= diff * delta;
  }
  else if (m_b1->isKinematic) {
    p0 += diff * delta;
  }
  else {
    p0 += diff * (delta * w0);
    p1 -= diff * (delta * w1);
  }
}

void DistanceConstraint::draw()
{
  assert(m_b0 != nullptr && m_b1 != nullptr);
  Draw::line(m_b0->pos(), m_b1->pos(), {0, 0, 0, 255});
}