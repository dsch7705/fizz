#include "fizz/Body.h"
#include "fizz/Constants.h"
#include "fizz/Draw.h"
#include "fizz/System.h"
#include "fizz/Util.h"

#include <iostream>

Body::Body(const DVec2& pos, double radius, bool isKinematic, double mass)
    : m_id(Util::nextId()),
      m_pos(pos),
      m_lastPos(pos),
      m_velocity(DVec2(0.0)),
      radius(radius),
      mass(mass),
      isKinematic(isKinematic)
{
}

void Body::integrateVerlet(bool last)
{
  if (isKinematic) {
    m_lastPos = m_pos;
    m_velocity = DVec2(0.0);
    return;
  }

  DVec2 acceleration = (m_forces + m_constraintForces) / mass;
  DVec2 prev = m_pos;
  m_pos = m_pos * 2 - m_lastPos + acceleration * kPhysicStep * kPhysicStep;
  m_velocity = (m_pos - prev) / kPhysicStep;
  m_lastPos = prev;

  m_constraintForces = DVec2(0.0);
  if (last)
    m_forces = DVec2(0.0);
}

void Body::addForce(const DVec2& f)
{
  m_forces += f;
}

void Body::addConstraintForce(const DVec2& f)
{
  m_constraintForces += f;
}

void Body::addImpulse(const DVec2& j)
{
  DVec2 dv = j / mass;
  m_lastPos -= dv * kPhysicStep;
}

void Body::draw(Draw::Color color) const
{
  Draw::circle(m_pos, radius, color);
}

void Body::setPos(const DVec2& pos)
{
  m_pos = pos;
  m_lastPos = pos;
}