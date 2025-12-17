#include "fizz/Constants.h"
#include "fizz/System.h"
#include "fizz/constraints/PositionConstraint.h"
#include "fizz/constraints/SpringConstraint.h"

#include "../raylib_Draw.h"

int main(int argc, char** argv)
{
  Draw::setCircleFunc(raylib_circle);
  Draw::setLineFunc(raylib_line);

  System system;
  Body* b0 = system.createBody({kMetersWidth / 2, 0.5}, 0.5, false, 1.0);
  Body* b1 = system.createBody({kMetersWidth / 2, kMetersHeight / 2}, 0.5, false, 1.0);
  system.createConstraint<SpringConstraint>(b0, b1, -1.0, 150.0, 1.0);
  system.createConstraint<PositionConstraint>(DVec2(0.0, -100.0), DVec2(kMetersWidth, kMetersHeight), 0.6, b0, b1);

  InitWindow(kScreenWidth, kScreenHeight, "Pogo");
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    if (IsKeyDown(KEY_SPACE))
      b0->addForce(DVec2(0.0, 500.0));

    system.update(GetFrameTime());

    BeginDrawing();
    ClearBackground(RAYWHITE);
    system.draw();
    EndDrawing();
  }
}