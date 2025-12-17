#include "fizz/constraints/DistanceConstraint.h"

#include "fizz/Body.h"
#include "fizz/Draw.h"
#include "fizz/Util.h"

DistanceConstraint::DistanceConstraint(Body* b0, Body* b1, double distance) : PairConstraint(b0, b1), distance(distance)
{
  if (distance < 0.0) {
    distance = (m_b1->pos() - m_b0->pos()).mag();
  }
}

void DistanceConstraint::solve()
{
  assert(m_b0 != nullptr && m_b1 != nullptr);

  DVec2& v0 = m_b0->m_pos;
  DVec2& v1 = m_b1->m_pos;
  DVec2 diff = v1 - v0;
  double mag = diff.mag();
  diff.normalize();

  double w0 = 1.0 / m_b0->mass;
  double w1 = 1.0 / m_b1->mass;
  double wSum = w0 + w1;

  double delta = (mag - distance) / wSum;

  if (m_b0->isKinematic) {
    v1 -= diff * delta;
  }
  else if (m_b1->isKinematic) {
    v0 += diff * delta;
  }
  else {
    v0 += diff * (delta * w0);
    v1 -= diff * (delta * w1);
  }
}

void DistanceConstraint::draw()
{
  assert(m_b0 != nullptr && m_b1 != nullptr);
  Draw::line(m_b0->pos(), m_b1->pos(), {0, 0, 0, 255});
}