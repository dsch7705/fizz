#include "fizz/Constraint.h"
#include "fizz/System.h"
#include "fizz/Util.h"

#include <cassert>

PairConstraint::PairConstraint(Body* b0, Body* b1) : Constraint(), m_b0(b0), m_b1(b1)
{
  m_n = DVec2::normalize(b1->pos() - b0->pos());
}

void RangeConstraint::addSystem(System* system)
{
  assert(system != nullptr);

  for (auto& [_, body] : system->bodies()) {
    addBody(body.get());
  }
}