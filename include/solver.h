#pragma once

#include "dvec.h"

#include <iostream>

struct Color;
class Body {
 public:
  Body(const DVec2& pos, double radius, bool isKinematic = false, double mass = 1.0)
      : m_pos(pos),
        m_lastPos(pos),
        m_velocity({0.0, 0.0}),
        m_acceleration({0.0, 0.0}),
        m_radius(radius),
        m_mass(mass),
        m_isKinematic(isKinematic)
  {
  }

  void setPos(const DVec2& pos);
  DVec2 pos() const { return m_pos; }
  DVec2 lastPos() const { return m_lastPos; }
  DVec2 velocity() const { return m_velocity; }
  DVec2 acceleration() const { return m_acceleration; }

  double radius() const { return m_radius; }
  double mass() const { return m_mass; }
  bool isKinematic() const { return m_isKinematic; }

  void addImpulse(const DVec2& j);
  void addForce(DVec2 f);
  void accelerate(DVec2 a);
  void integrateVerlet();

  void draw(Color c) const;

 private:
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
  Constraint(Body* _b0, Body* _b1) : b0(_b0), b1(_b1) {}
  virtual ~Constraint() {};
  virtual void solve() = 0;
  virtual void draw() {}

  Body* b0;
  Body* b1;
};
struct DistanceConstraint : public Constraint {
  DistanceConstraint(Body* _b0, Body* _b1, double _distance = -1.0) : Constraint(_b0, _b1), distance(_distance)
  {
    if (distance < 0.0) {
      distance = (_b1->pos() - _b0->pos()).mag();
    }
  }

  double distance;

  void solve() override;
  void draw() override;
};
struct SpringConstraint : public Constraint {
  SpringConstraint(Body* _b0, Body* _b1, double _length = -1.0, double _k = 15.0, double _damping = 1.0)
      : Constraint(_b0, _b1), length(_length), k(_k), damping(_damping)
  {
    if (length < 0.0) {
      length = (_b1->pos() - _b0->pos()).mag();
    }
  }

  double length;
  double k;
  double damping;

  void solve() override;
  void draw() override;
};
struct PositionConstraint : public Constraint {
  PositionConstraint(Body* _b0, const DVec2& _minPos, const DVec2& _maxPos, double _bounce = 0.8)
      : Constraint(_b0, nullptr), minPos(_minPos), maxPos(_maxPos), bounce(_bounce)
  {
  }

  DVec2 minPos, maxPos;
  double bounce;

  void solve() override;
};
struct AngleConstraint : public Constraint {
  AngleConstraint(Body* _b0, Body* _b1, Body* _pivot, double _angle, double _stiffness)
      : Constraint(_b0, _b1), pivot(_pivot), angle(_angle), stiffness(_stiffness)
  {
  }

  Body* pivot;
  double angle;
  double stiffness;

  void solve() override;
};