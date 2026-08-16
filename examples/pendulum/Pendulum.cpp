#include "Pendulum.h"

#include "fizz/Constants.h"
#include "fizz/System.h"

#include "fizz/constraints/DistanceConstraint.h"
#include "fizz/constraints/SpringConstraint.h"

#include <cmath>

Pendulum::Pendulum(int nLinks, const Vec2& anchor, float distance) : System()
{
  ID lastBody = createBody(anchor, 0.1f, true);

  for (int i = 0; i < nLinks; ++i) {
    double angle = (std::rand() % 360) * (M_PI / 180.0f);
    Vec2 dir{cosf(angle) * distance, sinf(angle) * distance};

    ID currentBody = createBody(getBody(lastBody).pos() + dir, 0.1f);
    if (i == 0) {
      getBody(currentBody).mass = 1000.0;
      getBody(currentBody).radius = 0.3f;
    }

    createConstraint<DistanceConstraint>(currentBody, lastBody, 0.05f);

    lastBody = currentBody;
  }
}