#pragma once

#include "constants.h"
#include "solver.h"
#include "util.h"

#include "raylib.h"

#include <memory>
#include <unordered_map>
#include <vector>

class IKArm {
 public:
  IKArm(const DVec2& origin, unsigned int nLinks, double linkDist);

  void update(double dT, unsigned int iterations, DVec2 endEffector);
  void draw();

 private:
  std::vector<Body> m_bodies;
  std::vector<std::unique_ptr<DistanceConstraint>> m_constraints;
  double maxLength() const;
};

class Pendulum {
 public:
  Pendulum(int nLinks, const DVec2& anchor, double distance, bool springs = false);

  bool drawPath{true};
  void drop();

  bool go{true};
  void update(unsigned int constraintIterations);
  void draw(const RenderTexture2D& renderTex);

  double calcPotentialEnergy();
  double calcKineticEnergy();

  std::vector<Body>& bodies() { return m_bodies; }

 private:
  std::vector<Body> m_bodies;
  std::vector<std::unique_ptr<Constraint>> m_constraints;
};

class System {
 public:
  System(bool effectedByGravity = false) : m_effectedByGravity(effectedByGravity) {}

  int createBody(const DVec2& pos, double radius = 0.2, bool isKinematic = false, double mass = 1.0);
  Body* getBody(int id);

  template <typename T_Constraint, typename... Args>
  int createConstraint(int id0, int id1, Args... args)
  {
    static_assert(std::is_base_of_v<Constraint, T_Constraint>, "T_Constraint must derive from Constraint");
    Body* b0 = getBody(id0);
    Body* b1 = getBody(id1);

    int id = nextId();
    m_constraints.insert(std::make_pair(id, std::make_unique<T_Constraint>(b0, b1, args...)));
    return id;
  }
  template <typename T_Constraint, typename... Args>
  int createConstraint(int id0, Args... args)
  {
    static_assert(std::is_base_of_v<Constraint, T_Constraint>, "T_Constraint must derive from Constraint");
    Body* b0 = getBody(id0);

    int id = nextId();
    m_constraints.insert(std::make_pair(id, std::make_unique<T_Constraint>(b0, args...)));
    return id;
  }

  void draw() const;
  void update();

 private:
  int nextId()
  {
    static int id = 0;
    return id++;
  }

  std::unordered_map<int, Body> m_bodies;
  std::unordered_map<int, std::unique_ptr<Constraint>> m_constraints;

  bool m_effectedByGravity;
};