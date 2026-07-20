#include "fizz/Constants.h"
#include "fizz/System.h"
#include "fizz/constraints/PositionConstraint.h"
#include "fizz/constraints/SpringConstraint.h"

#include "../raylib_Draw.h"

int main(int argc, char** argv)
{
  Draw::setCircleCallback(raylib_circle);
  Draw::setLineCallback(raylib_line);

  constexpr int screenW = 640;
  constexpr int screenH = 480;

  Draw::Transform& transform = Draw::getTransform();
  transform.scale = 50;
  transform.offset = Vec2(screenW, screenH) / transform.scale / 2;

  Vec2 worldSize = Vec2(screenW, screenH) / transform.scale;

  System system;
  ID b0 = system.createBody({0, -worldSize.y / 2}, 0.5, false, 1.0);
  ID b1 = system.createBody({0, 0}, 0.5, false, 1.0);
  system.createConstraint<SpringConstraint>(b0, b1);
  system.createConstraint<PositionConstraint>(Vec2(-worldSize.x / 2, -worldSize.y * 2), worldSize / 2, 0.6, b0, b1);

  InitWindow(screenW, screenH, "Pogo");
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    if (IsKeyDown(KEY_SPACE))
      system.getBody(b0).addForce(Vec2(0.0, 250.0));

    system.update(GetFrameTime());

    BeginDrawing();
    ClearBackground(RAYWHITE);

    system.draw(Draw::Color{0, 0, 0, 255});
    DrawText("Use space to pogo", 15, 15, 30, BLACK);

    EndDrawing();
  }
}