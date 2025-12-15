#include <raylib.h>
#include <chrono>
#include <iostream>
#include <thread>

#include "constants.h"
#include "systems.h"

int main(int argc, char** argv)
{
  SetTraceLogLevel(LOG_ERROR);
  InitWindow(kScreenWidth, kScreenHeight, "Fizz");
  std::srand(std::time(0));

  double accumulator{0};

  RenderTexture2D bg = LoadRenderTexture(kScreenWidth, kScreenHeight);
  BeginTextureMode(bg);
  ClearBackground(BLACK);
  EndTextureMode();

  double startTime{0};
  double elapsedTime{0};

  // Stuff
  DVec2 anchor{kMetersWidth / 2.0, kMetersHeight / 2.0};

  const int nLinks{3};
  const double dist = (kMetersHeight / 2.0) / nLinks * .9;

  Pendulum p(nLinks, anchor, dist, false);
  IKArm arm(anchor, nLinks, dist);

  System sys(true);
  int id0 = sys.createBody({kMetersWidth / 2, kMetersHeight / 4}, 0.3, false);
  int id1 = sys.createBody({kMetersWidth / 4, 0.0}, 0.3, false);
  int id2 = sys.createBody({(kMetersWidth / 4) * 3, 0.0}, 0.3);
  int id3 = sys.createBody({(kMetersWidth / 4) * 3, kMetersHeight / 3}, 0.5);

  sys.createConstraint<SpringConstraint>(id0, id1, -1.0, 300.0, 1.0);
  sys.createConstraint<SpringConstraint>(id1, id2, -1.0, 500.0, 1.0);
  sys.createConstraint<SpringConstraint>(id2, id0, -1.0, 300.0, 1.0);
  sys.createConstraint<PositionConstraint>(id3, DVec2(0.0, 0.0), DVec2(kMetersWidth, kMetersHeight));
  sys.createConstraint<PositionConstraint>(id2, DVec2(0.0, 0.0), DVec2(kMetersWidth, kMetersHeight));
  sys.createConstraint<PositionConstraint>(id1, DVec2(0.0, 0.0), DVec2(kMetersWidth, kMetersHeight));
  sys.createConstraint<PositionConstraint>(id0, DVec2(0.0, 0.0), DVec2(kMetersWidth, kMetersHeight));

  while (!WindowShouldClose()) {
    startTime = GetTime();
    if (IsKeyPressed(KEY_SPACE)) {
      DVec2 doohickyImpulse(std::rand() % 300 - 150, std::rand() % 400 - 200);
      sys.getBody(id0)->addImpulse(doohickyImpulse / 2);
      sys.getBody(id3)->addImpulse(doohickyImpulse / 4);
      p.bodies().back().addImpulse(doohickyImpulse / 10);
    }

    DVec2 mousePos{static_cast<double>(GetMouseX()), static_cast<double>(GetMouseY())};

    while (accumulator >= kPhysicStep) {
      p.update(1);
      sys.update();

      accumulator -= kPhysicStep;
    }

    BeginDrawing();

    // DrawTexture(bg.texture, 0, 0, WHITE);
    ClearBackground(GRAY);
    p.draw(bg);
    sys.draw();
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
