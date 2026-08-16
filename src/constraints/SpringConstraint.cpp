#include "fizz/constraints/SpringConstraint.h"

#include "fizz/Body.h"
#include "fizz/Constants.h"
#include "fizz/Draw.h"
#include "fizz/System.h"

#include <cmath>

SpringConstraint::SpringConstraint(System* system, ID id, ID b0, ID b1, float k, float damping, float width)
    : PairConstraint(system, id, b0, b1, width), k(k), damping(damping)
{
  const auto& body0 = system->getBody(b0);
  const auto& body1 = system->getBody(b1);
  length = (body1.m_pos - body0.m_pos).mag();
}

void SpringConstraint::solve()
{
  // TODO: centeralize this logic for all constraints
  if (!isEnabled)
    return;

  Body& body0 = m_system->getBody(m_b0);
  Body& body1 = m_system->getBody(m_b1);

  if (body0.isKinematic && body1.isKinematic)
    return;

  Vec2 p0 = body0.pos();
  Vec2 p1 = body1.pos();
  Vec2 diff = p1 - p0;
  float mag = diff.mag();
  if (mag != 0)
    m_n = diff / mag;

  // Hooke's Law
  float x = mag - length;
  float Fs = -k * x;

  Vec2 v0 = body0.velocity();
  Vec2 v1 = body1.velocity();
  Vec2 vrel = v1 - v0;

  float c = damping;
  if (c < 0.0) {  // Apply critical damping if the damping factor is below zero
    float m = (body0.mass * body1.mass) / (body0.mass + body1.mass);
    c = 2 * sqrt(k * m);
  }
  float Fd = -c * vrel.dot(m_n);

  float force = Fs + Fd;
  if (body0.isKinematic) {
    body1.addConstraintForce(diff * force);
  }
  else if (body1.isKinematic) {
    body0.addConstraintForce(-diff * force);
  }
  else {
    body0.addConstraintForce(-diff * (force / 2));
    body1.addConstraintForce(diff * (force / 2));
  }
}

void SpringConstraint::draw(Draw::Color color) const
{
  // TODO: centralize for all constraints
  if (!isVisible)
    return;

  const auto& body0 = m_system->getBody(m_b0);
  const auto& body1 = m_system->getBody(m_b1);

  Vec2 p0 = body0.pos();
  Vec2 p1 = body1.pos();
  Vec2 n = p1 - p0;
  float mag = n.mag();
  n.normalize();
  Vec2 perp = {-n.y, n.x};

  Draw::line(p0, p1, width * Draw::getTransform().scale, color);

  // constexpr float coil_spacing = .3f;
  // float half_w = std::min(m_b0->radius, m_b1->radius);
  // int n_coils = std::min(length / coil_spacing, 2.);

  // Vec2 last_p0;
  // for (int i = 0; i < n_coils; i++) {
  //   float t = (mag / n_coils) * i;
  //   Vec2 line_center = p0 + n * t;
  //   Vec2 line_p0 = line_center + perp * half_w;
  //   Vec2 line_p1 = line_center - perp * half_w;
  //   Draw::line(line_p0, line_p1, color);
  //   if (i != 0)
  //     Draw::line(last_p0, line_p1, color);
  //
  //   last_p0 = line_p0;
  // }
}
