#include "systems.h"
#include "util.h"

#include "raylib.h"

#include <cmath>
#include <iostream>

IKArm::IKArm(const DVec2& origin, unsigned int nLinks, double linkDist)
{
  m_bodies.reserve(nLinks + 1);
  m_bodies.emplace_back(origin, 0.75, false);

  for (int i = 0; i < nLinks; ++i) {
    m_bodies.emplace_back(m_bodies[i].pos() + DVec2{linkDist, 0}, 0.25);
    m_constraints.push_back(std::make_unique<DistanceConstraint>(&m_bodies[i], &m_bodies[i + 1], linkDist));
  }
}

void IKArm::update(double dT, unsigned int iterations, DVec2 endEffector)
{
  static double maxLen = maxLength();

  // FABRIK
  for (int i = 0; i < iterations; ++i) {
    if ((endEffector - m_bodies.front().pos()).mag() > maxLen) {
      endEffector = m_bodies.front().pos() + DVec2::normalize(endEffector - m_bodies.front().pos()) * maxLen;
    }
    // Forward pass
    m_bodies.back().pos() = endEffector;
    for (auto it = m_constraints.begin(); it != m_constraints.end(); ++it) {
      it->get()->solve();
    }
    // Backward pass
    for (auto it = m_constraints.rbegin(); it != m_constraints.rend(); ++it) {
      it->get()->solve();
    }
  }
}
void IKArm::draw()
{
  for (int i = 0; i < m_bodies.size(); ++i) {
    Body& b0 = m_bodies[i];
    DVec2 b0_screenSpace = worldToScreen(b0.pos());

    if (i + 1 < m_bodies.size()) {
      Body& b1 = m_bodies[i + 1];
      DVec2 b1_screenSpace = worldToScreen(b1.pos());
      DrawLine(b0_screenSpace.x, b0_screenSpace.y, b1_screenSpace.x, b1_screenSpace.y, BLACK);
    }

    DrawCircle(b0_screenSpace.x, b0_screenSpace.y, 15, BLUE);
  }
}

double IKArm::maxLength() const
{
  double sum = 0;
  for (const auto& constraint : m_constraints) {
    sum += constraint->distance;
  }

  return sum;
}

Pendulum::Pendulum(int nLinks, const DVec2& anchor, double distance, bool springs)
{
  m_bodies.reserve(nLinks + 1);

  for (int i = 0; i < nLinks + 1; ++i) {
    if (i == 0) {
      m_bodies.emplace_back(anchor, 0.25, 1.0, true);
    }
    else {
      double angle = (std::rand() % 360) * (PI / 180.0);
      DVec2 dir{cos(angle) * distance, sin(angle) * distance};
      m_bodies.emplace_back(m_bodies.back().pos() + dir, 0.25);
      if (springs)
        m_constraints.push_back(
            std::make_unique<SpringConstraint>(&m_bodies[i - 1], &m_bodies[i], distance, 150, -1.0));
      else
        m_constraints.push_back(std::make_unique<DistanceConstraint>(&m_bodies[i - 1], &m_bodies[i], distance));
    }
  }
}

void Pendulum::drop()
{
  m_constraints.clear();
  drawPath = false;
}

void Pendulum::update(unsigned int constraintIterations)
{
  if (!go) {
    return;
  }

  for (int i = 0; i < constraintIterations; ++i) {
    for (auto& constraint : m_constraints) {
      constraint->solve();
    }
  }
  for (Body& body : m_bodies) {
    body.accelerate(DVec2(0.0, kGravity));
    body.integrateVerlet();
  }
}

void Pendulum::draw(const RenderTexture2D& renderTex)
{
  if (m_bodies.empty()) {
    return;
  }

  for (int i = 0; i < m_bodies.size(); ++i) {
    Body& b0 = m_bodies[i];
    // b0.draw(RED);

    if (drawPath) {
      BeginTextureMode(renderTex);
      static Color pathCol1{0, 127, 255, 5};

      Color pathCol;
      switch (i % 2) {
        case 0:
          pathCol = pathCol1;
          break;
        case 1:
          pathCol = pathCol1;
          break;
      }
      b0.draw(pathCol);
      EndTextureMode();
    }
  }

  for (const auto& constraint : m_constraints) {
    constraint->draw();
  }
}

double Pendulum::calcPotentialEnergy()
{
  double energy = 0.0;
  for (const Body& body : m_bodies) {
    double h = kMetersHeight - body.pos().y;
    energy += body.mass() * kGravity * h;
  }

  return energy;
}

double Pendulum::calcKineticEnergy()
{
  double energy = 0.0;
  for (const Body& body : m_bodies) {
    double v = body.velocity().mag();
    energy += 0.5 * body.mass() * v * v;
  }

  return energy;
}

int System::createBody(const DVec2& pos, double radius, bool isKinematic, double mass)
{
  int id = nextId();

  Body body(pos, radius, isKinematic);
  m_bodies.emplace(id, body);
  return id;
}
Body* System::getBody(int id)
{
  if (!m_bodies.contains(id))
    return nullptr;

  return &m_bodies.at(id);
}

void System::draw() const
{
  for (auto& [_, constraint] : m_constraints) {
    constraint->draw();
  }

  for (const auto& [_, body] : m_bodies) {
    body.draw(BLACK);
  }
}

void System::update()
{
  for (auto& [_, constraint] : m_constraints) {
    constraint->solve();
  }

  for (auto& [_, body] : m_bodies) {
    if (m_effectedByGravity)
      body.accelerate(DVec2(0.0, kGravity));

    body.integrateVerlet();
  }
}