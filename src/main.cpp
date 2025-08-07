#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>
#include <raylib.h>

#include "solver.h"
#include "systems.h"
#include "util.h"


constexpr double kPixelsPerMeter { 60.0 };
constexpr double kMetersWidth { 15.0 };
constexpr double kMetersHeight { 15.0 };
constexpr double kScreenWidth { kMetersWidth * kPixelsPerMeter };
constexpr double kScreenHeight { kMetersHeight * kPixelsPerMeter };

constexpr DVec2 kAccGravity { .x = 0.0, .y = 98.0665 };

constexpr unsigned int kTargetFPS { 0 };
constexpr double kTargetFrametime { kTargetFPS ? 1.0 / static_cast<double>(kTargetFPS) : 0 };
constexpr double kPhysicStep { 0.00001 };

DVec2 worldToScreen(DVec2 coord)
{
    return coord * kPixelsPerMeter;
}
DVec2 screenToWorld(DVec2 coord)
{
    return coord * (1.0 / kPixelsPerMeter);
}

class Pendulum
{
public:
    Pendulum(int nLinks, const DVec2& anchor, double distance)
    {
        m_bodies.reserve(nLinks + 1);

        for (int i = 0; i < nLinks + 1; ++i)
        {
            if (i == 0)
            {
                m_bodies.emplace_back(anchor, 0.25, true);
            }
            else
            {
                double angle = (std::rand() % 360) * (PI / 180.0);
                DVec2 dir { cos(angle) * distance, sin(angle) * distance };
                m_bodies.emplace_back(m_bodies.back().pos + dir, 0.25);
                m_constraints.push_back(std::make_unique<DistanceConstraint>(&m_bodies[i - 1], &m_bodies[i], distance));
            }
        }
    }

    bool drawPath { true };
    void drop()
    {
        m_constraints.clear();
        drawPath = false;
    }
    bool go { false };
    void update(double dT, unsigned int constraintIterations)
    {
        if (!go)
        {
            return;
        }

        for (Body& body : m_bodies)
        {
            body.integrateVerlet(kAccGravity, dT);
        }
        for (int i = 0; i < constraintIterations; ++i)
        {
            for (auto& constraint : m_constraints)
            {
                constraint->solve();
            }
        }
    }
    void draw(const RenderTexture2D& renderTex)
    {
        if (m_bodies.empty())
        {
            return;
        }
        
        for (int i = 0; i < m_bodies.size(); ++i)
        {
            Body& b0 = m_bodies[i];
            DVec2 b0_screenSpace = worldToScreen(b0.pos);

            //if (i + 1 < m_bodies.size())
            //{
            //    Body& b1 = m_bodies[i + 1];
            //    DVec2 b1_screenSpace = worldToScreen(b1.pos);
            //    DrawLine(b0_screenSpace.x, b0_screenSpace.y, b1_screenSpace.x, b1_screenSpace.y, BLACK);
            //}
            
            //if (i < 2)
            //{
            //    continue;
            //}

            //DrawCircle(b0_screenSpace.x, b0_screenSpace.y, b0.radius * kPixelsPerMeter, WHITE);

            if (drawPath)
            {
                BeginTextureMode(renderTex);
                static Color pathCol1 { 255, 95, 30, 1 };
                static Color pathCol2 { 255, 0, 255, 1 };
                //static Color pathCol3 { 0, 255, 0, 1 };

                Color pathCol;
                switch (i % 2)
                {
                case 0:
                    pathCol = pathCol1;
                    break;
                case 1:
                    pathCol = pathCol2;
                    break;
                }
                DrawCircle(b0_screenSpace.x, kScreenHeight - b0_screenSpace.y, 2, pathCol);
                EndTextureMode(); 
            }
        }        
    }

private:
    std::vector<Body> m_bodies;
    std::vector<std::unique_ptr<Constraint>> m_constraints;
};

int main(int argc, char** argv)
{
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(kScreenWidth, kScreenHeight, "Gravity");
    std::srand(std::time(0));

    double accumulator { 0 };

    DVec2 anchor { kMetersWidth / 2.0, kMetersHeight / 2.0 };

    const int nLinks { 4 };
    const double dist = (kMetersHeight / 2.0) / nLinks;

    //Pendulum p(nLinks, anchor, dist);
    //IKArm arm(anchor, nLinks, dist * 0.75);

    RenderTexture2D bg = LoadRenderTexture(kScreenWidth, kScreenHeight);
    BeginTextureMode(bg);
    ClearBackground(BLACK);
    EndTextureMode();

    double startTime { 0 };
    double elapsedTime { 0 };

    Body b0({ kMetersWidth / 2.0, kMetersHeight / 2.0 }, 0.25);
    Body bPivot({ kMetersWidth / 2.0 + 2.0, kMetersHeight / 2.0 }, 0.15, true);
    Body b1({ kMetersWidth / 2.0 + 2.0, kMetersHeight / 2.0 + 2.0 }, 0.25);
    AngleConstraint ac(&b0, &b1, &bPivot, PI / 4, 0.2);
    DistanceConstraint dc0(&b0, &bPivot, 2.0);
    DistanceConstraint dc1(&bPivot, &b1, 2.0);

    while(!WindowShouldClose())
    {
        startTime = GetTime();
        //if (IsKeyPressed(KEY_SPACE))
        //{
        //    if (p.go)
        //    {
        //        p.drop();
        //    }
        //    else
        //    {
        //        p.go = true;
        //    }
        //}

        DVec2 mousePos { static_cast<double>(GetMouseX()), static_cast<double>(GetMouseY()) };

        while (accumulator >= kPhysicStep)
        {
            //p.update(kPhysicStep, 1);
            //arm.update(kPhysicStep, 1, screenToWorld(mousePos));

            b0.integrateVerlet(kAccGravity, kPhysicStep);
            b1.integrateVerlet(kAccGravity, kPhysicStep);

            ac.solve();
            dc0.solve();
            dc1.solve();

            accumulator -= kPhysicStep;
        }

        BeginDrawing();

        //DrawTexture(bg.texture, 0, 0, WHITE);
        //p.draw(bg);
        ClearBackground(GRAY);
        //arm.draw();

        //std::string fpsStr = "FPS: " + std::to_string(1.0 / elapsedTime);
        //DrawText(fpsStr.c_str(), 10, 10, 16, LIME);

        DVec2 b0_screenSpace = worldToScreen(b0.pos);
        DVec2 bPivot_screenSpace = worldToScreen(bPivot.pos);
        DVec2 b1_screenSpace = worldToScreen(b1.pos);

        DrawLine(b0_screenSpace.x, b0_screenSpace.y, bPivot_screenSpace.x, bPivot_screenSpace.y, BLACK);
        DrawLine(bPivot_screenSpace.x, bPivot_screenSpace.y, b1_screenSpace.x, b1_screenSpace.y, BLACK);


        EndDrawing();
        
        elapsedTime = GetTime() - startTime;
        if (elapsedTime < kTargetFrametime)
        {
            int millis = (kTargetFrametime - elapsedTime) * 1000;
            std::this_thread::sleep_for(std::chrono::milliseconds(millis));
            elapsedTime = kTargetFrametime;
        }
        accumulator += elapsedTime;
    }

    UnloadRenderTexture(bg);
    CloseWindow();
}