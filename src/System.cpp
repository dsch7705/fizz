#include "fizz/System.h"
#include "fizz/Constants.h"

#include <cassert>

Body* System::createBody(const DVec2& pos, double radius, bool isKinematic, double mass)
{
  auto body = std::make_unique<Body>(pos, radius, isKinematic, mass);
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

void System::draw(Draw::Color color) const
{
  for (auto& [_, constraint] : m_constraints) {
    constraint->draw(color);
  }

  for (const auto& [_, body] : m_bodies) {
    body->draw(color);
  }
}

void System::update(double dT)
{
  while (dT >= kPhysicStep) {
    for (auto& [_, constraint] : m_constraints) {
      constraint->solve();
    }

    for (auto& [_, body] : m_bodies) {
      if (effectedByGravity)
        body->addConstraintForce(DVec2(0.0, kGravity * body->mass));

      body->integrateVerlet(dT - kPhysicStep < kPhysicStep);
    }

    dT -= kPhysicStep;
  }
}

void System::clear()
{
  m_bodies.clear();
  m_constraints.clear();
}

void System::removeConstraint(int id)
{
  m_constraints.erase(id);
}