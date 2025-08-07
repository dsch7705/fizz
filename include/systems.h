#include "solver.h"

#include <memory>
#include <vector>


class IKArm
{
public:
    IKArm(const DVec2& origin, unsigned int nLinks, double linkDist);

    void update(double dT, unsigned int iterations, DVec2 endEffector);
    void draw();

private:
    std::vector<Body> m_bodies;
    std::vector<std::unique_ptr<DistanceConstraint>> m_constraints;
    double maxLength() const;

};