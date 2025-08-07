#include "solver.h"


void Body::integrateVerlet(const DVec2& acc, double dT)
{
    if (isKinematic)
    {
        return;
    }

    DVec2 tmpPos = pos;
    pos = (pos * 2) - lastPos + (acc * dT * dT);
    lastPos = tmpPos;
}
void DistanceConstraint::solve()
{
    DVec2* v0 = &b0->pos;
    DVec2* v1 = &b1->pos;
    DVec2 diff = *v1 - *v0;
    double mag = diff.mag();
    diff.normalize();
    
    double delta = mag - distance;

    if (b0->isKinematic)
    {
        *v1 -= diff * delta;
    }
    else if (b1->isKinematic)
    {
        *v0 += diff * delta;
    }
    else
    {
        *v0 += diff * (delta / 2);
        *v1 -= diff * (delta / 2);
    }
}