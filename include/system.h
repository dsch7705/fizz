#pragma once

#include "constants.h"
#include "solver.h"
#include "util.h"

#include "raylib.h"

#include <memory>
#include <unordered_map>
#include <vector>

class System {
 private:
  inline static std::unordered_map<int, std::unique_ptr<System>> m_systems;

 public:
  System() : m_id(nextId()) {}

  template <typename T = System, typename... Args>
    requires std::derived_from<T, System>
  static T* createSystem(Args... args)
  {
    auto system = std::make_unique<T>(args...);
    T* ptr = system.get();
    m_systems.emplace(system->m_id, std::move(system));

    return ptr;
  }
  static System* getSystem(int id);

  Body* createBody(const DVec2& pos, double radius = 0.2, bool isKinematic = false, double mass = 1.0);
  Body* getBody(int id);

  template <typename T, typename... Args>
    requires std::derived_from<T, Constraint>
  T* createConstraint(Args... args)
  {
    auto constraint = std::make_unique<T>(args...);
    T* ptr = constraint.get();
    m_constraints.emplace(constraint->id(), std::move(constraint));

    return ptr;
  }

  virtual void draw() const;
  virtual void update();

  const std::unordered_map<int, std::unique_ptr<Body>>& bodies() const { return m_bodies; }
  const std::unordered_map<int, std::unique_ptr<Constraint>>& constraints() const { return m_constraints; }

  void setEffectedByGravity(bool effected) { m_effectedByGravity = effected; }
  void toggleGravity() { m_effectedByGravity = !m_effectedByGravity; }
  bool effectedByGravity() const { return m_effectedByGravity; }

  const int id() const { return m_id; }

  static int nextId()
  {
    static int nextId = 0;
    return nextId++;
  }

 protected:
  const int m_id;
  bool m_effectedByGravity{true};

 private:
  std::unordered_map<int, std::unique_ptr<Body>> m_bodies;
  std::unordered_map<int, std::unique_ptr<Constraint>> m_constraints;
};

class Pendulum : public System {
 public:
  Pendulum(int nLinks, const DVec2& anchor, double distance, bool springs = false);

  double calcPotentialEnergy();
  double calcKineticEnergy();

  Body* head() const { return m_head; }
  Body* tail() const { return m_tail; }

 private:
  Body* m_head{nullptr};
  Body* m_tail{nullptr};
};