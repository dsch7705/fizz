#pragma once

#include "DVec.h"
#include "Util.h"

#include <cassert>
#include <concepts>
#include <set>

class Body;
class System;

struct Constraint {
  Constraint() : m_id(Util::nextId()) {}
  virtual ~Constraint() {}
  virtual void solve() = 0;
  virtual void draw() {}

  const int id() const { return m_id; }

 protected:
  const int m_id;
};

struct PairConstraint : public Constraint {
  PairConstraint(Body* b0, Body* b1);
  Body* b0() const { return m_b0; }
  Body* b1() const { return m_b1; }

 protected:
  Body* m_b0;
  Body* m_b1;

  // Normalized direction vector from b0 -> b1
  DVec2 m_n;
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
    auto check = [](auto* b) {
      if (!b)
        throw std::invalid_argument("Body cannot be null");
    };
    (check(bodies), ...);

    (m_bodies.insert(bodies), ...);
  }

  void addSystem(System* system);

 protected:
  std::set<Body*> m_bodies;
};