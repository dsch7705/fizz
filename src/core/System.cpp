#include "fizz/core/System.h"
#include "fizz/core/Constants.h"

#include <cassert>

System* System::getSystem(int id)
{
  if (!m_systems.contains(id))
    return nullptr;

  return m_systems.at(id).get();
}

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

void System::draw() const
{
  for (auto& [_, constraint] : m_constraints) {
    constraint->draw();
  }

  for (const auto& [_, body] : m_bodies) {
    body->draw();
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