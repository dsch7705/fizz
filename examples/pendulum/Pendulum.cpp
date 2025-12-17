#include "Pendulum.h"

#include "fizz/Constants.h"
#include "fizz/System.h"

#include "fizz/constraints/DistanceConstraint.h"
#include "fizz/constraints/SpringConstraint.h"

#include <cmath>

Pendulum::Pendulum(int nLinks, const DVec2& anchor, double distance, bool springs) : System()
{
  Body* lastBody = createBody(anchor, 0.25, true);
  m_head = lastBody;

  for (int i = 0; i < nLinks; ++i) {
    double angle = (std::rand() % 360) * (kPi / 180.0);
    DVec2 dir{cos(angle) * distance, sin(angle) * distance};

    Body* currentBody = createBody(lastBody->pos() + dir, 0.25);
    if (i == 0) {
      currentBody->mass = 1000.0;
      currentBody->radius = 0.5;
    }

    if (springs)
      createConstraint<SpringConstraint>(currentBody, lastBody);
    else
      createConstraint<DistanceConstraint>(currentBody, lastBody);

    lastBody = currentBody;
  }

  m_tail = lastBody;
}

double Pendulum::calcPotentialEnergy()
{
  double energy = 0.0;
  for (const auto& [_, body] : bodies()) {
    double h = kMetersHeight - body->pos().y;
    energy += body->mass * kGravity * h;
  }

  return energy;
}

double Pendulum::calcKineticEnergy()
{
  double energy = 0.0;
  for (const auto& [_, body] : bodies()) {
    double v = body->velocity().mag();
    energy += 0.5 * body->mass * v * v;
  }

  return energy;
}