#include "system.h"
#include "util.h"

#include "raylib.h"

#include <cassert>
#include <cmath>
#include <iostream>

// FABRIK
// for (int i = 0; i < iterations; ++i) {
//  if ((endEffector - m_bodies.front().pos()).mag() > maxLen) {
//    endEffector = m_bodies.front().pos() + DVec2::normalize(endEffector - m_bodies.front().pos()) * maxLen;
//  }
//  // Forward pass
//  m_bodies.back().pos() = endEffector;
//  for (auto it = m_constraints.begin(); it != m_constraints.end(); ++it) {
//    it->get()->solve();
//  }
//  // Backward pass
//  for (auto it = m_constraints.rbegin(); it != m_constraints.rend(); ++it) {
//    it->get()->solve();
//  }
//}

Pendulum::Pendulum(int nLinks, const DVec2& anchor, double distance, bool springs) : System()
{
  Body* lastBody = createBody(anchor, 0.25, true);
  m_head = lastBody;

  for (int i = 0; i < nLinks; ++i) {
    double angle = (std::rand() % 360) * (PI / 180.0);
    DVec2 dir{cos(angle) * distance, sin(angle) * distance};

    Body* currentBody = createBody(lastBody->pos() + dir, 0.25);
    if (springs)
      createConstraint<SpringConstraint>(currentBody, lastBody, distance, 150.0, 1.0);
    else
      createConstraint<DistanceConstraint>(currentBody, lastBody, distance);

    lastBody = currentBody;
  }

  m_tail = lastBody;
}

double Pendulum::calcPotentialEnergy()
{
  double energy = 0.0;
  for (const auto& [_, body] : bodies()) {
    double h = kMetersHeight - body->pos().y;
    energy += body->mass() * kGravity * h;
  }

  return energy;
}

double Pendulum::calcKineticEnergy()
{
  double energy = 0.0;
  for (const auto& [_, body] : bodies()) {
    double v = body->velocity().mag();
    energy += 0.5 * body->mass() * v * v;
  }

  return energy;
}

System* System::getSystem(int id)
{
  if (!m_systems.contains(id))
    return nullptr;

  return m_systems.at(id).get();
}

Body* System::createBody(const DVec2& pos, double radius, bool isKinematic, double mass)
{
  auto body = std::make_unique<Body>(this, pos, radius, isKinematic);
  Body* ptr = body.get();
  m_bodies.emplace(body->id(), std::move(body));

  return ptr;
}
Body* System::getBody(int id)
{
  if (!m_bodies.contains(id))
    return nullptr;

  return m_bodies.at(id).get();
}

void System::draw() const
{
  for (auto& [_, constraint] : m_constraints) {
    constraint->draw();
  }

  for (const auto& [_, body] : m_bodies) {
    body->draw(BLACK);
  }
}

void System::update()
{
  for (auto& [_, constraint] : m_constraints) {
    constraint->solve();
  }

  for (auto& [_, body] : m_bodies) {
    if (m_effectedByGravity)
      body->accelerate(DVec2(0.0, kGravity));

    body->integrateVerlet();
  }
}