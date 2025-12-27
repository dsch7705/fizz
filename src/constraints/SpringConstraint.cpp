#include "fizz/constraints/SpringConstraint.h"

#include "fizz/Body.h"
#include "fizz/Constants.h"
#include "fizz/Draw.h"

#include <cmath>

SpringConstraint::SpringConstraint(Body* b0, Body* b1, double k, double damping)
    : PairConstraint(b0, b1), k(k), damping(damping)
{
  length = (b1->pos() - b0->pos()).mag();
}

void SpringConstraint::solve()
{
  assert(m_b0 != nullptr && m_b1 != nullptr);

  if (m_b0->isKinematic && m_b1->isKinematic)
    return;

  DVec2 p0 = m_b0->pos();
  DVec2 p1 = m_b1->pos();
  DVec2 diff = p1 - p0;
  double mag = diff.mag();
  if (mag != 0)
    m_n = diff / mag;

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
  double Fd = -c * vrel.dot(m_n);

  double force = Fs + Fd;
  if (m_b0->isKinematic) {
    m_b1->addConstraintForce(diff * force);
  }
  else if (m_b1->isKinematic) {
    m_b0->addConstraintForce(-diff * force);
  }
  else {
    m_b0->addConstraintForce(-diff * (force / 2));
    m_b1->addConstraintForce(diff * (force / 2));
  }
}

void SpringConstraint::draw(Draw::Color color) const
{
  assert(m_b0 != nullptr && m_b1 != nullptr);

  DVec2 p0 = m_b0->pos();
  DVec2 p1 = m_b1->pos();
  Draw::line(p0, p1, color);
  // DVec2 n = p1 - p0;
  // double mag = n.mag();
  // n.normalize();
  // DVec2 perp = {-n.y, n.x};
  //
  // constexpr float coil_spacing = 15.f;
  // float half_w = std::min(m_b0->radius, m_b1->radius);
  //
  // int n_coils = (length * kPixelsPerMeter) / coil_spacing;
  // for (int i = 0; i < n_coils; i++) {
  //  double t = (mag / n_coils) * i;
  //  DVec2 line_center = p0 + n * t;
  //  DVec2 line_p0 = line_center + perp * half_w;
  //  DVec2 line_p1 = line_center - perp * half_w;
  //  Draw::line(line_p0, line_p1, {0, 0, 0, 255});
  //}
}