#pragma once

#include "../core/System.h"

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