#include "fizz/constraints/SpringConstraint.h"

#include "fizz/Body.h"
#include "fizz/Constants.h"
#include "fizz/Draw.h"
#include "fizz/System.h"

#include <cmath>

SpringConstraint::SpringConstraint(System* system, ID id, ID b0, ID b1, double k, double damping)
    : PairConstraint(system, id, b0, b1), k(k), damping(damping)
{
  const auto& body0 = system->getBody(b0);
  const auto& body1 = system->getBody(b1);
  length = (body1.m_pos - body0.m_pos).mag();
}

void SpringConstraint::solve()
{
  Body& body0 = m_system->getBody(m_b0);
  Body& body1 = m_system->getBody(m_b1);

  if (body0.isKinematic && body1.isKinematic)
    return;

  DVec2 p0 = body0.pos();
  DVec2 p1 = body1.pos();
  DVec2 diff = p1 - p0;
  double mag = diff.mag();
  if (mag != 0)
    m_n = diff / mag;

  // Hooke's Law
  double x = mag - length;
  double Fs = -k * x;

  DVec2 v0 = body0.velocity();
  DVec2 v1 = body1.velocity();
  DVec2 vrel = v1 - v0;

  double c = damping;
  if (c < 0.0) {  // Apply critical damping if the damping factor is below zero
    double m = (body0.mass * body1.mass) / (body0.mass + body1.mass);
    c = 2 * sqrt(k * m);
  }
  double Fd = -c * vrel.dot(m_n);

  double force = Fs + Fd;
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
  const auto& body0 = m_system->getBody(m_b0);
  const auto& body1 = m_system->getBody(m_b1);

  DVec2 p0 = body0.pos();
  DVec2 p1 = body1.pos();
  DVec2 n = p1 - p0;
  double mag = n.mag();
  n.normalize();
  DVec2 perp = {-n.y, n.x};

  Draw::line(p0, p1, color);

  // constexpr float coil_spacing = .3f;
  // float half_w = std::min(m_b0->radius, m_b1->radius);
  // int n_coils = std::min(length / coil_spacing, 2.);

  // DVec2 last_p0;
  // for (int i = 0; i < n_coils; i++) {
  //   double t = (mag / n_coils) * i;
  //   DVec2 line_center = p0 + n * t;
  //   DVec2 line_p0 = line_center + perp * half_w;
  //   DVec2 line_p1 = line_center - perp * half_w;
  //   Draw::line(line_p0, line_p1, color);
  //   if (i != 0)
  //     Draw::line(last_p0, line_p1, color);
  //
  //   last_p0 = line_p0;
  // }
}
