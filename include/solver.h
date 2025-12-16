#pragma once

#include "dvec.h"

#include <cassert>
#include <iostream>
#include <set>
#include <vector>

struct Color;

class System;
class Body {
 public:
  Body(System* system, const DVec2& pos, double radius, bool isKinematic = false, double mass = 1.0);

  void setPos(const DVec2& pos);
  DVec2 pos() const { return m_pos; }
  DVec2 lastPos() const { return m_lastPos; }
  DVec2 velocity() const { return m_velocity; }
  DVec2 acceleration() const { return m_acceleration; }
  int id() const { return m_id; }

  double radius() const { return m_radius; }
  double mass() const { return m_mass; }
  bool isKinematic() const { return m_isKinematic; }

  void addImpulse(const DVec2& j);
  void addForce(DVec2 f);
  void accelerate(DVec2 a);
  void integrateVerlet();

  void draw(Color c) const;

 private:
  int m_id;

  DVec2 m_pos;
  DVec2 m_lastPos;
  DVec2 m_velocity;
  DVec2 m_acceleration;

  double m_radius;
  double m_mass;
  bool m_isKinematic;

  friend class DistanceConstraint;
  friend class SpringConstraint;
  friend class PositionConstraint;
  friend class AngleConstraint;
};

struct Constraint {
  Constraint();
  virtual ~Constraint() {}
  virtual void solve() = 0;
  virtual void draw() {}

  int id() const { return m_id; }

 protected:
  int m_id;
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

struct DistanceConstraint : public PairConstraint {
  DistanceConstraint(Body* b0, Body* b1, double _distance = -1.0) : PairConstraint(b0, b1), distance(_distance)
  {
    if (distance < 0.0) {
      distance = (m_b1->pos() - m_b0->pos()).mag();
    }
  }

  double distance;

  void solve() override;
  void draw() override;
};
struct SpringConstraint : public PairConstraint {
  SpringConstraint(Body* b0, Body* b1, double _length = -1.0, double _k = 15.0, double _damping = 1.0)
      : PairConstraint(b0, b1), length(_length), k(_k), damping(_damping)
  {
    if (length < 0.0) {
      length = (b1->pos() - b0->pos()).mag();
    }
  }

  double length;
  double k;
  double damping;

  void solve() override;
  void draw() override;
};
struct PositionConstraint : public RangeConstraint {
  template <typename... Bs>
    requires(std::same_as<Bs, Body*> && ...)
  PositionConstraint(const DVec2& _minPos, const DVec2& _maxPos, double _bounce, Bs... bodyIds)
      : RangeConstraint(bodyIds...), minPos(_minPos), maxPos(_maxPos), bounce(_bounce)
  {
  }

  DVec2 minPos, maxPos;
  double bounce;

  void solve() override;
};