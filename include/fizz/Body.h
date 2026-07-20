#pragma once

#include "Constants.h"
#include "Draw.h"
#include "Vec2.h"

class System;
class Body {
 public:
  Body(ID id, const Vec2& pos, float radius, bool isKinematic = false, float mass = 1.0);

  void setPos(const Vec2& pos);
  Vec2 pos() const { return m_pos; }
  Vec2 lastPos() const { return m_lastPos; }
  Vec2 velocity() const { return m_velocity; }

  const ID id() const { return m_id; }

  float radius;
  float mass;
  bool isKinematic;

  // Adds a force to be applied this simulation step; cleared every step
  void addForce(const Vec2& f);
  // Adds a force to be applied this simulation sub-step; cleared every sub-step
  void addConstraintForce(const Vec2& f);
  // Adds an immediate change in velocity
  void addImpulse(const Vec2& j);

  // last should evaluate to true on the final solver iteration
  void integrateVerlet(bool last);

  virtual void draw(Draw::Color color) const;

 private:
  const ID m_id;

  Vec2 m_pos;
  Vec2 m_lastPos;
  Vec2 m_velocity;

  // Forces to be applied this frame; cleared each frame
  Vec2 m_forces{Vec2(0.0)};
  // Constraint forces to be applied this physics substep; cleared each substep
  Vec2 m_constraintForces{Vec2(0.0)};

  friend class DistanceConstraint;
  friend class SpringConstraint;
  friend class PositionConstraint;
};