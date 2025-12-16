#include "fizz/constraint/SpringConstraint.h"

#include "fizz/core/Body.h"
#include "fizz/core/Constants.h"
#include "fizz/core/Util.h"

#include "raylib.h"

#include <cmath>

SpringConstraint::SpringConstraint(Body* b0, Body* b1, double length, double k, double damping)
    : PairConstraint(b0, b1), length(length), k(k), damping(damping)
{
  if (length < 0.0) {
    length = (b1->pos() - b0->pos()).mag();
  }
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
    DVec2 line_center = Util::worldToScreen(p0 + n * t);
    DVec2 line_p0 = line_center + perp * half_w;
    DVec2 line_p1 = line_center - perp * half_w;
    DrawLine(line_p0.x, line_p0.y, line_p1.x, line_p1.y, BLACK);
  }

  DVec2 screen_p0 = Util::worldToScreen(p0);
  DVec2 screen_p1 = Util::worldToScreen(p1);
  DrawCircle(screen_p0.x, screen_p0.y, half_w / 2, BLACK);
  DrawCircle(screen_p1.x, screen_p1.y, half_w / 2, BLACK);
}