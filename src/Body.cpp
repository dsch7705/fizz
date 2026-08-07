#include "fizz/Body.h"
#include "fizz/Constants.h"
#include "fizz/Draw.h"
#include "fizz/System.h"
#include "fizz/Util.h"

#include <iostream>

Body::Body(ID id, const Vec2& pos, float radius, bool isKinematic, float mass)
    : m_id(id),
      m_pos(pos),
      m_lastPos(pos),
      m_velocity(Vec2(0.0)),
      radius(radius),
      mass(mass),
      isKinematic(isKinematic),
      isVisible(true)
{
}

void Body::integrateVerlet(bool last)
{
  if (isKinematic) {
    m_lastPos = m_pos;
    m_velocity = Vec2(0.0);
    return;
  }

  Vec2 acceleration = (m_forces + m_constraintForces) / mass;
  Vec2 prev = m_pos;
  m_pos = m_pos * 2 - m_lastPos + acceleration * kPhysicStep * kPhysicStep;
  m_velocity = (m_pos - prev) / kPhysicStep;
  m_lastPos = prev;

  m_constraintForces = Vec2(0.0);
  if (last)
    m_forces = Vec2(0.0);
}

void Body::addForce(const Vec2& f)
{
  m_forces += f;
}

void Body::addConstraintForce(const Vec2& f)
{
  m_constraintForces += f;
}

void Body::addImpulse(const Vec2& j)
{
  Vec2 dv = j / mass;
  m_lastPos -= dv * kPhysicStep;
}

void Body::draw(Draw::Color color) const
{
  if (!isVisible)
    return;

  Draw::circle(m_pos, radius, color);
}

void Body::setPos(const Vec2& pos)
{
  assert(isKinematic && "Cannot explicitly set the position of a non-kinematic body.");
  if (!isKinematic)
    return;

  m_lastPos = m_pos;
  m_pos = pos;
}