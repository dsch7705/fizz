#pragma once

#include <cassert>
#include <concepts>
#include <set>

class Body;
class System;

struct Constraint {
  Constraint();
  virtual ~Constraint() {}
  virtual void solve() = 0;
  virtual void draw() {}

  const int id() const { return m_id; }

 protected:
  const int m_id;
};

struct PairConstraint : public Constraint {
  PairConstraint(Body* b0, Body* b1) : Constraint(), m_b0(b0), m_b1(b1) {}

 protected:
  Body* m_b0;
  Body* m_b1;
};

struct RangeConstraint : public Constraint {
  template <typename... Ts>
    requires(std::same_as<Ts, Body*> && ...)
  RangeConstraint(Ts... bodies) : Constraint(), m_bodies({bodies...})
  {
  }

  template <typename... Bs>
    requires(std::same_as<Bs, Body*> && ...)
  void addBody(Bs... bodies)
  {
    (assert(bodies != nullptr), ...);
    (m_bodies.insert(bodies), ...);
  }

  void addSystem(System* system);

 protected:
  std::set<Body*> m_bodies;
};