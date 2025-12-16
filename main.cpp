#include <raylib.h>
#include <chrono>
#include <iostream>
#include <thread>

#include "fizz/core/Constants.h"
#include "fizz/core/System.h"

#include "fizz/constraint/DistanceConstraint.h"
#include "fizz/constraint/PositionConstraint.h"
#include "fizz/constraint/SpringConstraint.h"

#include "fizz/systems/Pendulum.h"

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

  const int nLinks{2};
  const double dist = (kMetersHeight / 2.0) / nLinks * .9;
  Pendulum* p = System::createSystem<Pendulum>(nLinks, anchor, dist, false);

  System* sys = System::createSystem();

  Body* b0 = sys->createBody({kMetersWidth / 2, kMetersHeight / 2}, 0.3, false);
  Body* b1 = sys->createBody({kMetersWidth / 4, kMetersHeight / 4}, 0.3, false);
  Body* b2 = sys->createBody({(kMetersWidth / 4) * 3, kMetersHeight / 4}, 0.3);
  Body* b3 = sys->createBody({(kMetersWidth / 4) * 3, kMetersHeight / 3}, 0.5);
  Body* b4 = sys->createBody({kMetersWidth / 2, 0}, 0.3, false);

  // Varying mass
  System* sys2 = System::createSystem();
  sys2->setEffectedByGravity(true);

  double k = 1500.0;
  double d = -1.0;

  Body* b5 = sys2->createBody({kMetersWidth / 3, 0}, 0.4, false, 100.0);
  Body* b6 = sys2->createBody({kMetersWidth / 3, kMetersHeight / 2}, 0.4, false, 1.0);
  sys2->createConstraint<SpringConstraint>(b5, b6, -1.0, k, d);

  Body* b7 = sys2->createBody({(kMetersWidth / 3) * 2, 0}, 0.4, false, 1.0);
  Body* b8 = sys2->createBody({(kMetersWidth / 3) * 2, kMetersHeight / 2}, 0.4, false, 10.0);
  sys2->createConstraint<SpringConstraint>(b7, b8, -1.0, k, d);

  // double doohickyDamping = 0.3;
  // sys->createConstraint<SpringConstraint>(b0, b1, -1.0, 150.0, doohickyDamping);
  // sys->createConstraint<SpringConstraint>(b1, b2, -1.0, 250.0, doohickyDamping);
  // sys->createConstraint<SpringConstraint>(b2, b0, -1.0, 150.0, doohickyDamping);
  // sys->createConstraint<SpringConstraint>(b4, b1, -1.0, 150, doohickyDamping);
  // sys->createConstraint<SpringConstraint>(b4, b2, -1.0, 150, doohickyDamping);
  // sys->createConstraint<SpringConstraint>(b4, b0, -1.0, 250, doohickyDamping);
  // sys->createConstraint<DistanceConstraint>(b0, b1);
  sys->createConstraint<SpringConstraint>(b1, b2, -1.0, 250);
  sys->createConstraint<DistanceConstraint>(b2, b0);
  sys->createConstraint<DistanceConstraint>(b4, b1);
  sys->createConstraint<DistanceConstraint>(b4, b2);

  PositionConstraint borderConstraint(DVec2(0.0), DVec2(kMetersWidth, kMetersHeight), 0.8);
  borderConstraint.addSystem(p);
  borderConstraint.addSystem(sys);
  borderConstraint.addSystem(sys2);

  while (!WindowShouldClose()) {
    startTime = GetTime();
    if (IsKeyPressed(KEY_SPACE)) {
      // DVec2 doohickyImpulse(std::rand() % 300 - 150, std::rand() % 400 - 200);
      // b0->addImpulse(doohickyImpulse / 2);
      // b3->addImpulse(doohickyImpulse / 24);
      // p->tail()->addImpulse(doohickyImpulse / 32);
      p->head()->isKinematic = !p->head()->isKinematic;
    }
    if (IsKeyPressed(KEY_G)) {
      p->toggleGravity();
      sys->toggleGravity();
    }

    DVec2 mousePos{static_cast<double>(GetMouseX()), static_cast<double>(GetMouseY())};

    while (accumulator >= kPhysicStep) {
      borderConstraint.solve();
      p->update();
      // sys->update();
      // sys2->update();

      accumulator -= kPhysicStep;
    }

    BeginDrawing();
    ClearBackground(GRAY);

    p->draw();
    // sys->draw();
    // sys2->draw();

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