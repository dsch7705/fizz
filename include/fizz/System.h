#pragma once

#include "Body.h"
#include "Constraint.h"
#include "Draw.h"
#include "Util.h"

#include "civ.hpp"
#include "fizz/constraints/DistanceConstraint.h"
#include "fizz/constraints/PositionConstraint.h"
#include "fizz/constraints/SpringConstraint.h"

#include <memory>
#include <variant>

class Body;

using ConstraintVariant = std::variant<DistanceConstraint, PositionConstraint, SpringConstraint>;

inline Constraint& constraintBase(ConstraintVariant& v)
{
  return std::visit([](auto& obj) -> Constraint& { return obj; }, v);
}
inline const Constraint& constraintBase(const ConstraintVariant& v)
{
  return std::visit([](const auto& obj) -> const Constraint& { return obj; }, v);
}

class System {
 public:
  System() : m_id(Util::nextId()) {}

  bool effectedByGravity{true};
  float gravity{-9.81f};

  ID createBody(const Vec2& pos, float radius = 0.2, bool isKinematic = false, float mass = 1.0);
  Body& getBody(ID id);

  Constraint& getConstraintBase(ID id) { return constraintBase(m_constraints[id]); }

  template <typename T, typename... Args>
    requires std::derived_from<T, Constraint>
  ID createConstraint(Args... args)
  {
    return m_constraints.emplace_back(T(this, m_constraints.getNextID(), std::forward<Args>(args)...));
  }
  void removeConstraint(ID id);

  virtual void draw(Draw::Color color) const;
  void update(float dT);
  void clear();

  const auto& bodies() const { return m_bodies; }
  const auto& constraints() const { return m_constraints; }

  const int id() const { return m_id; }

 protected:
  const int m_id;

 private:
  civ::IndexVector<Body> m_bodies;
  civ::IndexVector<ConstraintVariant> m_constraints;

  // std::unordered_map<int, std::unique_ptr<Body>> m_bodies;
  // std::unordered_map<int, std::unique_ptr<Constraint>> m_constraints;
};