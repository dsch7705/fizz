#include "fizz/Constraint.h"
#include "fizz/System.h"
#include "fizz/Util.h"

#include <cassert>

Constraint::Constraint() : m_id(Util::nextId()) {}

void RangeConstraint::addSystem(System* system)
{
  assert(system != nullptr);

  for (auto& [_, body] : system->bodies()) {
    addBody(body.get());
  }
}