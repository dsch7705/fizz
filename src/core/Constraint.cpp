#include "fizz/core/Constraint.h"
#include "fizz/core/System.h"

#include <cassert>

Constraint::Constraint() : m_id(System::nextId()) {}

void RangeConstraint::addSystem(System* system)
{
  assert(system != nullptr);

  for (auto& [_, body] : system->bodies()) {
    addBody(body.get());
  }
}