#include "Pendulum.h"

#include "fizz/Constants.h"
#include "fizz/Draw.h"

#include "../raylib_Draw.h"

#include <iostream>

int main(int argc, char** argv)
{
  Draw::setCircleFunc(raylib_circle);
  Draw::setLineFunc(raylib_line);

  constexpr int screenW = 640;
  constexpr int screenH = 480;

  Draw::Transform& transform = Draw::getTransform();
  transform.scale = 50;
  transform.offset = DVec2(screenW, screenH) / transform.scale / 2;

  int links = 2;
  double distance = screenH / transform.scale / 2 / links * 0.9;
  Pendulum p(2, DVec2(0, 0), distance, false);

  InitWindow(screenW, screenH, "Pendulum");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    p.update(GetFrameTime());

    BeginDrawing();
    ClearBackground(RAYWHITE);
    p.draw(Draw::Color{0, 0, 0, 255});
    EndDrawing();
  }
}