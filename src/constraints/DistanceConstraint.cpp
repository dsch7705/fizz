#include "fizz/constraints/DistanceConstraint.h"

#include "fizz/Body.h"
#include "fizz/Draw.h"
#include "fizz/System.h"

DistanceConstraint::DistanceConstraint(System* system, ID id, ID b0, ID b1) : PairConstraint(system, id, b0, b1)
{
  const auto& body0 = system->getBody(b0);
  const auto& body1 = system->getBody(b1);
  distance = (body1.pos() - body0.pos()).mag();
}

void DistanceConstraint::solve()
{
  if (!isEnabled)
    return;

  Body& body0 = m_system->getBody(m_b0);
  Body& body1 = m_system->getBody(m_b1);

  auto& p0 = body0.m_pos;
  auto& p1 = body1.m_pos;
  Vec2 diff = p1 - p0;
  float mag = diff.mag();
  diff.normalize();

  float w0 = 1.0 / body0.mass;
  float w1 = 1.0 / body1.mass;
  float wSum = w0 + w1;

  float delta = (mag - distance) / wSum;

  if (body0.isKinematic) {
    p1 -= diff * delta;
  }
  else if (body1.isKinematic) {
    p0 += diff * delta;
  }
  else {
    p0 += diff * (delta * w0);
    p1 -= diff * (delta * w1);
  }
}

void DistanceConstraint::draw(Draw::Color color) const
{
  if (!isVisible)
    return;

  Body& body0 = m_system->getBody(m_b0);
  Body& body1 = m_system->getBody(m_b1);
  Draw::line(body0.pos(), body1.pos(), color);
}