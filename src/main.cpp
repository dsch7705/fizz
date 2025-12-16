#include <raylib.h>
#include <chrono>
#include <iostream>
#include <thread>

#include "constants.h"
#include "system.h"

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
  Pendulum* p = System::createSystem<Pendulum>(nLinks, anchor, dist, false);

  System* sys = System::createSystem();

  Body* b0 = sys->createBody({kMetersWidth / 2, kMetersHeight / 2}, 0.3, false);
  Body* b1 = sys->createBody({kMetersWidth / 4, kMetersHeight / 4}, 0.3, false);
  Body* b2 = sys->createBody({(kMetersWidth / 4) * 3, kMetersHeight / 4}, 0.3);
  Body* b3 = sys->createBody({(kMetersWidth / 4) * 3, kMetersHeight / 3}, 0.5);
  Body* b4 = sys->createBody({kMetersWidth / 2, 0}, 0.3, false);

  double doohickyDamping = 0.3;
  sys->createConstraint<SpringConstraint>(b0, b1, -1.0, 150.0, doohickyDamping);
  sys->createConstraint<SpringConstraint>(b1, b2, -1.0, 250.0, doohickyDamping);
  sys->createConstraint<SpringConstraint>(b2, b0, -1.0, 150.0, doohickyDamping);
  sys->createConstraint<SpringConstraint>(b4, b1, -1.0, 150, doohickyDamping);
  sys->createConstraint<SpringConstraint>(b4, b2, -1.0, 150, doohickyDamping);
  sys->createConstraint<SpringConstraint>(b4, b0, -1.0, 250, doohickyDamping);

  auto borderConstraint = sys->createConstraint<PositionConstraint>(DVec2(0.0), DVec2(kMetersWidth, kMetersHeight),
                                                                    0.95, b0, b1, b2, b3, b4);
  borderConstraint->addSystem(p);

  while (!WindowShouldClose()) {
    startTime = GetTime();
    if (IsKeyPressed(KEY_SPACE)) {
      DVec2 doohickyImpulse(std::rand() % 300 - 150, std::rand() % 400 - 200);
      b0->addImpulse(doohickyImpulse / 2);
      b3->addImpulse(doohickyImpulse / 24);
      p->tail()->addImpulse(doohickyImpulse / 32);
    }
    if (IsKeyPressed(KEY_G)) {
      p->toggleGravity();
      sys->toggleGravity();
    }

    DVec2 mousePos{static_cast<double>(GetMouseX()), static_cast<double>(GetMouseY())};

    while (accumulator >= kPhysicStep) {
      p->update();
      sys->update();

      accumulator -= kPhysicStep;
    }

    BeginDrawing();
    ClearBackground(GRAY);

    p->draw();
    sys->draw();

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