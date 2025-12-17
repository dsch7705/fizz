#pragma once

#include "Body.h"
#include "Constraint.h"
#include "Util.h"

#include <memory>
#include <unordered_map>

class Body;

class System {
 private:
  // inline static std::unordered_map<int, std::unique_ptr<System>> m_systems;

 public:
  System() : m_id(Util::nextId()) {}

  bool effectedByGravity{true};

  //// Creates a System registered to be updated in the physics loop
  // template <typename T = System, typename... Args>
  //   requires std::derived_from<T, System>
  // static T* createSystem(Args... args)
  //{
  //   auto system = std::make_unique<T>(args...);
  //   T* ptr = system.get();
  //   m_systems.emplace(system->m_id, std::move(system));
  //
  //  return ptr;
  //}
  // static System* getSystem(int id);

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
  void update(double dT);

  const std::unordered_map<int, std::unique_ptr<Body>>& bodies() const { return m_bodies; }
  const std::unordered_map<int, std::unique_ptr<Constraint>>& constraints() const { return m_constraints; }

  const int id() const { return m_id; }

 protected:
  const int m_id;

 private:
  std::unordered_map<int, std::unique_ptr<Body>> m_bodies;
  std::unordered_map<int, std::unique_ptr<Constraint>> m_constraints;
};