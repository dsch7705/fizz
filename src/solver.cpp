#include "solver.h"

#include <cmath>
#include <iostream>
#include <algorithm>
#include <raylib.h>


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
    DVec2& v0 = b0->pos;
    DVec2& v1 = b1->pos;
    DVec2 diff = v1 - v0;
    double mag = diff.mag();
    diff.normalize();
    
    double delta = mag - distance;

    if (b0->isKinematic)
    {
        v1 -= diff * delta;
    }
    else if (b1->isKinematic)
    {
        v0 += diff * delta;
    }
    else
    {
        v0 += diff * (delta / 2);
        v1 -= diff * (delta / 2);
    }
}

void AngleConstraint::solve()
{
    const DVec2& vPivot = pivot->pos;
    DVec2 v0 = b0->pos - vPivot;
    DVec2 v1 = b1->pos - vPivot;

    double theta = atan2(v0.x * v1.y - v0.y * v1.x, v0.dot(v1) / (v0.mag() * v1.mag()));
    double delta = (angle - theta) * stiffness;

    if (b0->isKinematic)
    {
        v1.rotate(delta);
        b1->pos = v1 + vPivot;
    }
    else if (b1->isKinematic)
    {
        v0.rotate(-delta);
        b0->pos = v0 + vPivot;
    }
    else
    {
        v0.rotate(-delta / 2.0);
        v1.rotate(delta / 2.0);
        b0->pos = v0 + vPivot;
        b1->pos = v1 + vPivot;
    }
    //double theta = atan2(v0.x * v1.y - v0.y * v1.x, v0.dot(v1) / (v0.mag() * v1.mag()));
    //double delta = angle - theta;
//
    //// wrap delta to [-pi, pi]
    //delta = fmod(delta + PI, 2 * PI);
    //if (delta < 0) delta += 2 * PI;
    //delta -= PI;
//
    //// clamp and scale delta
    //constexpr double maxDelta = 0.05;
    //if (delta > maxDelta) delta = maxDelta;
    //else if (delta < -maxDelta) delta = -maxDelta;
    //delta *= 0.2;
//
    //// apply rotations, check sign if needed
    //v0.rotate(delta / 2.0);
    //v1.rotate(-delta / 2.0);
//
    //b0->pos = v0 + vPivot;
    //b1->pos = v1 + vPivot;
//
}