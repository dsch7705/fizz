#include "Pendulum.h"

#include "fizz/Constants.h"
#include "fizz/Draw.h"

#include "../raylib_Draw.h"

int main(int argc, char** argv)
{
  Draw::setCircleFunc(raylib_circle);
  Draw::setLineFunc(raylib_line);

  int links = 2;
  double distance = (kMetersHeight / 2) / 2 * 0.9;
  Pendulum p(2, DVec2(kMetersWidth / 2, kMetersHeight / 2), distance, false);

  InitWindow(kScreenWidth, kScreenHeight, "Pendulum");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    p.update(GetFrameTime());

    BeginDrawing();
    ClearBackground(RAYWHITE);
    p.draw(Draw::Color{0, 0, 0, 255});
    EndDrawing();
  }
}