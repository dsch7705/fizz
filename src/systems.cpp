#include "systems.h"
#include "util.h"

#include <raylib.h>


IKArm::IKArm(const DVec2& origin, unsigned int nLinks, double linkDist)
{
    m_bodies.reserve(nLinks + 1);
    m_bodies.emplace_back(origin, 0.25, true);

    for (int i = 0; i < nLinks; ++i)
    {
        m_bodies.emplace_back(m_bodies[i].pos + DVec2{ linkDist, 0 }, 0.25);
        m_constraints.push_back(std::make_unique<DistanceConstraint>(&m_bodies[i], &m_bodies[i + 1], linkDist));
    }
}

void IKArm::update(double dT, unsigned int iterations, DVec2 endEffector)
{
    static double maxLen = maxLength();

    // FABRIK
    for (int i = 0; i < iterations; ++i)
    {
        if ((endEffector - m_bodies.front().pos).mag() > maxLen)
        {
            endEffector = m_bodies.front().pos + DVec2::normalize(endEffector - m_bodies.front().pos) * maxLen;
        }
        // Forward pass
        m_bodies.back().pos = endEffector;
        for (auto it = m_constraints.begin(); it != m_constraints.end(); ++it)
        {
            it->get()->solve();
        }
        // Backward pass
        for (auto it = m_constraints.rbegin(); it != m_constraints.rend(); ++it)
        {
            it->get()->solve();
        }
    }
}
void IKArm::draw()
{
    for (int i = 0; i < m_bodies.size(); ++i)
    {
        Body& b0 = m_bodies[i];
        DVec2 b0_screenSpace = worldToScreen(b0.pos);

        if (i + 1 < m_bodies.size())
        {
            Body& b1 = m_bodies[i + 1];
            DVec2 b1_screenSpace = worldToScreen(b1.pos);
            DrawLine(b0_screenSpace.x, b0_screenSpace.y, b1_screenSpace.x, b1_screenSpace.y, BLACK);
        }

        DrawCircle(b0_screenSpace.x, b0_screenSpace.y, 5, WHITE);
    }
}

double IKArm::maxLength() const
{
    double sum = 0;
    for (const auto& constraint : m_constraints)
    {
        sum += constraint->distance;
    }

    return sum;
}