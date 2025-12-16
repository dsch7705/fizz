#include "solver.h"
#include "constants.h"
#include "system.h"
#include "util.h"

#include "raylib.h"

#include <cassert>
#include <cmath>
#include <iostream>

Body::Body(const DVec2& pos, double radius, bool isKinematic, double mass)
    : m_id(System::nextId()),
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
void Body::draw(Color c) const
{
  DVec2 screenPos = worldToScreen(m_pos);
  DrawCircle(screenPos.x, screenPos.y, radius * kPixelsPerMeter, c);
}
void Body::setPos(const DVec2& pos)
{
  m_pos = pos;
  m_lastPos = pos;
}

Constraint::Constraint() : m_id(System::nextId()) {}

void RangeConstraint::addSystem(System* system)
{
  assert(system != nullptr);

  for (auto& [_, body] : system->bodies()) {
    addBody(body.get());
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

  DVec2 p0 = worldToScreen(m_b0->m_pos);
  DVec2 p1 = worldToScreen(m_b1->m_pos);
  DrawLine(p0.x, p0.y, p1.x, p1.y, BLACK);
}

void SpringConstraint::solve()
{
  assert(m_b0 != nullptr && m_b1 != nullptr);

  DVec2 p0 = m_b0->pos();
  DVec2 p1 = m_b1->pos();
  DVec2 diff = p1 - p0;
  double mag = diff.mag();
  if (mag == 0)
    return;

  DVec2 n = diff / mag;

  // Hooke's Law
  double x = mag - length;
  double Fs = -k * x;

  DVec2 v0 = m_b0->velocity();
  DVec2 v1 = m_b1->velocity();
  DVec2 vrel = v1 - v0;

  double c = damping;
  if (c < 0.0) {  // Apply critical damping if the damping factor is below zero
    double m = (m_b0->mass * m_b1->mass) / (m_b0->mass + m_b1->mass);
    c = 2 * sqrt(k * m);
  }
  double Fd = -c * vrel.dot(n);

  double force = Fs + Fd;
  if (m_b0->isKinematic) {
    m_b1->addForce(diff * force);
  }
  else if (m_b1->isKinematic) {
    m_b0->addForce(-diff * force);
  }
  else {
    m_b0->addForce(-diff * (force / 2));
    m_b1->addForce(diff * (force / 2));
  }
}
void SpringConstraint::draw()
{
  assert(m_b0 != nullptr && m_b1 != nullptr);

  DVec2 p0 = m_b0->pos();
  DVec2 p1 = m_b1->pos();
  DVec2 n = p1 - p0;
  double mag = n.mag();
  n.normalize();
  DVec2 perp = {-n.y, n.x};

  constexpr float coil_spacing = 15.f;
  float half_w = std::min(m_b0->radius, m_b1->radius) * kPixelsPerMeter;

  int n_coils = (length * kPixelsPerMeter) / coil_spacing;
  for (int i = 0; i < n_coils; i++) {
    double t = (mag / n_coils) * i;
    DVec2 line_center = worldToScreen(p0 + n * t);
    DVec2 line_p0 = line_center + perp * half_w;
    DVec2 line_p1 = line_center - perp * half_w;
    DrawLine(line_p0.x, line_p0.y, line_p1.x, line_p1.y, BLACK);
  }

  DVec2 screen_p0 = worldToScreen(p0);
  DVec2 screen_p1 = worldToScreen(p1);
  DrawCircle(screen_p0.x, screen_p0.y, half_w / 2, BLACK);
  DrawCircle(screen_p1.x, screen_p1.y, half_w / 2, BLACK);
}

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