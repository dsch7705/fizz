#pragma once

#include "DVec.h"

class System;
class Body {
 public:
  Body(const DVec2& pos, double radius, bool isKinematic = false, double mass = 1.0);

  void setPos(const DVec2& pos);
  DVec2 pos() const { return m_pos; }
  DVec2 lastPos() const { return m_lastPos; }
  DVec2 velocity() const { return m_velocity; }
  DVec2 acceleration() const { return m_acceleration; }
  const int id() const { return m_id; }

  double radius;
  double mass;
  bool isKinematic;

  void addImpulse(const DVec2& j);
  void addForce(DVec2 f);
  void accelerate(DVec2 a);
  void integrateVerlet();

  void draw() const;

 private:
  const int m_id;

  DVec2 m_pos;
  DVec2 m_lastPos;
  DVec2 m_velocity;
  DVec2 m_acceleration;

  friend class DistanceConstraint;
  friend class SpringConstraint;
  friend class PositionConstraint;
  friend class AngleConstraint;
};