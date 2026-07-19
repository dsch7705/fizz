#include "fizz/System.h"
#include "fizz/Constants.h"

#include <cassert>
#include <iostream>

ID System::createBody(const DVec2& pos, double radius, bool isKinematic, double mass)
{
  // auto body = std::make_unique<Body>(pos, radius, isKinematic, mass);
  // Body* ptr = body.get();
  ID id = m_bodies.emplace_back(m_bodies.getNextID(), pos, radius, isKinematic, mass);
  return id;
}

Body& System::getBody(ID id)
{
  // if (!m_bodies.contains(id))
  //   return nullptr;

  // return m_bodies.at(id).get();
  return m_bodies[id];
}

void System::draw(Draw::Color color) const
{
  for (auto& constraint : m_constraints) {
    const auto& base = constraintBase(constraint);
    base.draw(color);
  }

  for (const auto& body : m_bodies) {
    body.draw(color);
  }
}

void System::update(double dT)
{
  while (dT >= kPhysicStep) {
    for (auto& constraint : m_constraints) {
      auto& base = constraintBase(constraint);
      base.solve();
    }

    for (auto& body : m_bodies) {
      if (effectedByGravity)
        body.addConstraintForce(DVec2(0.0, gravity * body.mass));

      body.integrateVerlet(dT - kPhysicStep < kPhysicStep);
    }

    dT -= kPhysicStep;
  }
}

void System::clear()
{
  m_bodies.clear();
  m_constraints.clear();
}

void System::removeConstraint(ID id)
{
  m_constraints.erase(id);
}