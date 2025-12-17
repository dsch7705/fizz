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

  const int id() const { return m_id; }

  double radius;
  double mass;
  bool isKinematic;

  // Adds a force to be applied this simulation step; cleared every step
  void addForce(const DVec2& f);
  // Adds a force to be applied this simulation sub-step; cleared every sub-step
  void addConstraintForce(const DVec2& f);
  // Adds an immediate change in velocity
  void addImpulse(const DVec2& j);

  // last should evaluate to true on the final solver iteration
  void integrateVerlet(bool last);

  virtual void draw() const;

 private:
  const int m_id;

  DVec2 m_pos;
  DVec2 m_lastPos;
  DVec2 m_velocity;

  // Forces to be applied this frame; cleared each frame
  DVec2 m_forces;
  // Constraint forces to be applied this physics substep; cleared each substep
  DVec2 m_constraintForces;

  friend class DistanceConstraint;
  friend class SpringConstraint;
  friend class PositionConstraint;
};