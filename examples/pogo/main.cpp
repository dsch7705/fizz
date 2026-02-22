#include "fizz/Constants.h"
#include "fizz/System.h"
#include "fizz/constraints/PositionConstraint.h"
#include "fizz/constraints/SpringConstraint.h"

#include "../raylib_Draw.h"

int main(int argc, char** argv)
{
  Draw::setCircleFunc(raylib_circle);
  Draw::setLineFunc(raylib_line);

  constexpr int screenW = 640;
  constexpr int screenH = 480;
  
  Draw::Transform& transform = Draw::getTransform();
  transform.scale = 50;
  transform.offset = DVec2(screenW, screenH) / transform.scale / 2;

  DVec2 worldSize = DVec2(screenW, screenH) / transform.scale;

  System system;
  Body* b0 = system.createBody({0, -worldSize.y / 2}, 0.5, false, 1.0);
  Body* b1 = system.createBody({0, 0}, 0.5, false, 1.0);
  system.createConstraint<SpringConstraint>(b0, b1);
  system.createConstraint<PositionConstraint>(DVec2(-worldSize.x / 2, -worldSize.y * 2), worldSize / 2, 0.6, b0, b1);

  InitWindow(screenW, screenH, "Pogo");
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    if (IsKeyDown(KEY_SPACE))
      b0->addForce(DVec2(0.0, 250.0));

    system.update(GetFrameTime());

    BeginDrawing();
    ClearBackground(RAYWHITE);

    system.draw(Draw::Color{0, 0, 0, 255});
    DrawText("Use space to pogo", 15, 15, 30, BLACK);

    EndDrawing();
  }
}