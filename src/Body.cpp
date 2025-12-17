#include "fizz/Body.h"
#include "fizz/Constants.h"
#include "fizz/Draw.h"
#include "fizz/System.h"
#include "fizz/Util.h"

Body::Body(const DVec2& pos, double radius, bool isKinematic, double mass)
    : m_id(Util::nextId()),
      m_pos(pos),
      m_lastPos(pos),
      m_velocity({0.0, 0.0}),
      m_acceleration({0.0, 0.0}),
      radius(radius),
      mass(mass),
      isKinematic(isKinematic)
{
}

void Body::integrateVerlet()
{
  if (isKinematic) {
    m_lastPos = m_pos;
    m_velocity = {0, 0};
    m_acceleration = {0, 0};
    return;
  }

  DVec2 prev = m_pos;
  m_pos = m_pos * 2 - m_lastPos + m_acceleration * kPhysicStep * kPhysicStep;
  m_velocity = (m_pos - prev) / kPhysicStep;
  m_lastPos = prev;

  m_acceleration = {0.0, 0.0};
}
void Body::addForce(DVec2 f)
{
  // F = M * A
  // A = F / M

  m_acceleration += f / mass;
}
void Body::addImpulse(const DVec2& j)
{
  DVec2 dv = j / mass;
  m_lastPos -= dv * kPhysicStep;
}
void Body::accelerate(DVec2 a)
{
  m_acceleration += a;
}
void Body::draw() const
{
  Draw::circle(m_pos, radius, {0, 0, 0, 255});
}
void Body::setPos(const DVec2& pos)
{
  m_pos = pos;
  m_lastPos = pos;
}