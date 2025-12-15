#include <raylib.h>
#include <chrono>
#include <iostream>
#include <thread>

#include "constants.h"
#include "systems.h"

int main(int argc, char** argv)
{
  SetTraceLogLevel(LOG_ERROR);
  InitWindow(kScreenWidth, kScreenHeight, "Gravity");
  std::srand(std::time(0));

  double accumulator{0};

  DVec2 anchor{kMetersWidth / 2.0, kMetersHeight / 2.0};

  const int nLinks{3};
  const double dist = (kMetersHeight / 2.0) / nLinks * .9;

  Pendulum p(nLinks, anchor, dist, false);
  Body p_ext(p.bodies().back().pos() + DVec2{dist, -dist / 2}, 3);

  Pendulum p1(nLinks, anchor, dist, true);
  IKArm arm(anchor, nLinks, dist);

  RenderTexture2D bg = LoadRenderTexture(kScreenWidth, kScreenHeight);
  BeginTextureMode(bg);
  ClearBackground(BLACK);
  EndTextureMode();

  double startTime{0};
  double elapsedTime{0};

  Body sp0({kMetersWidth / 2 - 2.0, kMetersHeight / 3}, 0.25, 1.0, true);
  Body sp1({kMetersWidth / 2 + 2.0, kMetersHeight / 3}, 0.25);
  SpringConstraint spc0(&sp0, &sp1, 3.0, 150.0, 1);
  SpringConstraint spc1(&sp1, &p1.bodies().at(p1.bodies().size() - 1), 2.5);
  // DistanceConstraint spc1(&sp1, &p1.bodies().at(p1.bodies().size() - 1), 2.5);

  System sys(true);
  int id0 = sys.createBody({kMetersWidth / 2, kMetersHeight / 4}, 0.3, false);
  int id1 = sys.createBody({kMetersWidth / 4, 0.0}, 0.3, false);
  int id2 = sys.createBody({(kMetersWidth / 4) * 3, 0.0}, 0.3);
  int id3 = sys.createBody({(kMetersWidth / 4) * 3, kMetersHeight / 3});
  sys.getBody(id3)->addImpulse(DVec2(10.0, -50.0));
  sys.createConstraint<SpringConstraint>(id0, id1, -1.0, 300.0, 1.0);
  sys.createConstraint<SpringConstraint>(id1, id2, -1.0, 1000.0, 1.0);
  sys.createConstraint<SpringConstraint>(id2, id0, -1.0, 300.0, 1.0);
  sys.createConstraint<PositionConstraint>(id3, DVec2(0.0, 0.0), DVec2(kMetersWidth, kMetersHeight));
  sys.createConstraint<PositionConstraint>(id2, DVec2(0.0, 0.0), DVec2(kMetersWidth, kMetersHeight));
  sys.createConstraint<PositionConstraint>(id1, DVec2(0.0, 0.0), DVec2(kMetersWidth, kMetersHeight));
  sys.createConstraint<PositionConstraint>(id0, DVec2(0.0, 0.0), DVec2(kMetersWidth, kMetersHeight));

  while (!WindowShouldClose()) {
    startTime = GetTime();
    if (IsKeyPressed(KEY_SPACE)) {
      // if (p.go)
      //{
      //     p.drop();
      // }
      // else
      //{
      //     p.go = true;
      // }
    }

    DVec2 mousePos{static_cast<double>(GetMouseX()), static_cast<double>(GetMouseY())};

    while (accumulator >= kPhysicStep) {
      p.update(1);
      // p1.update(kPhysicStep, 1);
      // arm.update(kPhysicStep, 1, screenToWorld(mousePos));

      // spc0.solve();
      // sp0.integrateVerlet();
      // sp1.accelerate(DVec2(0.0, kGravity));
      // sp1.integrateVerlet();

      sys.update();

      accumulator -= kPhysicStep;
    }

    BeginDrawing();

    // DrawTexture(bg.texture, 0, 0, WHITE);
    ClearBackground(GRAY);
    // p.draw(bg);
    // p1.draw(bg);
    // arm.draw();

    // double potentialEnergy = p.calcPotentialEnergy();
    // std::string potEnergyStr = "Potential energy: " + std::to_string(potentialEnergy);
    // DrawText(potEnergyStr.c_str(), 10, 10, 16, BLACK);
    //
    // double kineticEnergy = p.calcKineticEnergy();
    // std::string kinEnergyStr = "Kinetic energy: " + std::to_string(kineticEnergy);
    // DrawText(kinEnergyStr.c_str(), 10, 25, 16, BLACK);
    //
    // std::string totalEnergyStr = "Total energy: " + std::to_string(potentialEnergy + kineticEnergy);
    // DrawText(totalEnergyStr.c_str(), 10, 40, 16, BLACK);

    // DVec2 bPivot_screenSpace = worldToScreen(bPivot.pos);
    // DVec2 b0_screenSpace = worldToScreen(b0.pos);
    // DVec2 b1_screenSpace = worldToScreen(b1.pos);
    // DVec2 b2_screenSpace = worldToScreen(b2.pos);
    // DVec2 b3_screenSpace = worldToScreen(b3.pos);
    //
    // DrawLine(b0_screenSpace.x, b0_screenSpace.y, bPivot_screenSpace.x,
    // bPivot_screenSpace.y, BLACK); DrawLine(bPivot_screenSpace.x,
    // bPivot_screenSpace.y, b1_screenSpace.x, b1_screenSpace.y, BLACK);
    // DrawLine(b1_screenSpace.x, b1_screenSpace.y, b2_screenSpace.x,
    // b2_screenSpace.y, BLACK); DrawLine(b1_screenSpace.x, b1_screenSpace.y,
    // b3_screenSpace.x, b3_screenSpace.y, BLACK);

    // sp0.draw(GREEN);
    // sp1.draw(GREEN);

    sys.draw();

    EndDrawing();

    elapsedTime = GetTime() - startTime;
    if (elapsedTime < kTargetFrametime) {
      int millis = (kTargetFrametime - elapsedTime) * 1000;
      std::this_thread::sleep_for(std::chrono::milliseconds(millis));
      elapsedTime = kTargetFrametime;
    }
    accumulator += elapsedTime;
  }

  UnloadRenderTexture(bg);
  CloseWindow();
}
