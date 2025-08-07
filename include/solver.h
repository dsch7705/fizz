#pragma once

#include "dvec.h"


class Body
{
public:
    Body(const DVec2& _pos, double _radius, bool _isKinematic = false) 
    : pos(_pos)
    , lastPos(_pos)
    , radius(_radius)
    , isKinematic(_isKinematic) {}

    DVec2 pos;
    DVec2 lastPos;
    
    double radius;
    bool isKinematic;
    
    void integrateVerlet(const DVec2& acc, double dT);
};

struct Constraint
{
    Constraint(Body* _b0, Body* _b1) : b0(_b0), b1(_b1) {}
    virtual ~Constraint() {};
    virtual void solve() = 0;

    Body* b0;
    Body* b1;
};
struct DistanceConstraint : public Constraint
{
    DistanceConstraint(Body* _b0, Body* _b1, double _distance) : Constraint(_b0, _b1), distance(_distance) {}
    double distance;

    void solve() override;
};