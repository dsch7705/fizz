#include "fizz/Constraint.h"
#include "fizz/System.h"
#include "fizz/Util.h"

#include <cassert>

PairConstraint::PairConstraint(System* system, ID id, ID b0, ID b1, float width)
    : Constraint(system, id), m_b0(b0), m_b1(b1), width(width)
{
  const auto& body0 = system->getBody(b0);
  const auto& body1 = system->getBody(b1);
  m_n = Vec2::normalize(body1.pos() - body0.pos());
}

void RangeConstraint::addSystem()
{
  for (auto& body : m_system->bodies()) {
    addBody(body.id());
  }
}