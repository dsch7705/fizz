#pragma once

#include "Constants.h"
#include "Draw.h"
#include "Util.h"
#include "Vec2.h"

#include <cassert>
#include <concepts>
#include <set>

class Body;
class System;

struct Constraint {
  Constraint(System* system, ID id) : m_system(system), m_id(id) {}
  virtual ~Constraint() {}
  virtual void solve() = 0;
  virtual void draw(Draw::Color color) const {}

  ID id() const { return m_id; }

 protected:
  System* m_system;

 private:
  ID m_id;
};

struct PairConstraint : public Constraint {
  PairConstraint(System* system, ID id, ID b0, ID b1);
  ID b0() const { return m_b0; }
  ID b1() const { return m_b1; }

 protected:
  ID m_b0;
  ID m_b1;

  // Normalized direction vector from b0 -> b1
  Vec2 m_n;
};

struct RangeConstraint : public Constraint {
  template <typename... Ts>
    requires(std::same_as<Ts, ID> && ...)
  RangeConstraint(System* system, ID id, Ts... bodies) : Constraint(system, id), m_bodies({bodies...})
  {
  }

  template <typename... IDs>
  void addBody(IDs... ids)
  {
    (m_bodies.push_back(ids), ...);
  }

  void addSystem();

 protected:
  std::vector<ID> m_bodies;
};