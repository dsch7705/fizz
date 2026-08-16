#include "Pendulum.h"

#include "fizz/Constants.h"
#include "fizz/System.h"

#include "fizz/constraints/DistanceConstraint.h"
#include "fizz/constraints/SpringConstraint.h"

#include <cmath>

Pendulum::Pendulum(int nLinks, const Vec2& anchor, float distance) : System()
{
  float bodyR = distance / 10.f;
  ID lastBody = createBody(anchor, bodyR, true);

  for (int i = 0; i < nLinks; ++i) {
    double angle = (std::rand() % 360) * (M_PI / 180.0f);
    Vec2 dir{cosf(angle) * distance, sinf(angle) * distance};

    ID currentBody = createBody(getBody(lastBody).pos() + dir, bodyR);
    if (i == 0) {
      getBody(currentBody).mass = 1000.0;
      getBody(currentBody).radius = bodyR * 3.f;
    }

    createConstraint<DistanceConstraint>(currentBody, lastBody, bodyR / 4.f);

    lastBody = currentBody;
  }
}