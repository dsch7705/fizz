#include "Pendulum.h"

#include "fizz/Constants.h"
#include "fizz/Draw.h"

#include "raylib.h"

void drawCircle(const DVec2& center, float radius, Draw::Color color)
{
  Color c;
  c.r = color.r;
  c.g = color.g;
  c.b = color.b;
  c.a = color.a;
  DrawCircle(center.x, center.y, radius, c);
}
void drawLine(const DVec2& p0, const DVec2& p1, Draw::Color color)
{
  Color c;
  c.r = color.r;
  c.g = color.g;
  c.b = color.b;
  c.a = color.a;
  DrawLine(p0.x, p0.y, p1.x, p1.y, c);
}

int main(int argc, char** argv)
{
  Draw::setCircleFunc(drawCircle);
  Draw::setLineFunc(drawLine);

  int links = 2;
  double distance = (kMetersHeight / 2) / 2 * 0.9;
  Pendulum p(2, DVec2(kMetersWidth / 2, kMetersHeight / 2), distance, false);

  InitWindow(kScreenWidth, kScreenHeight, "Pendulum");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    p.update(GetFrameTime());

    BeginDrawing();
    ClearBackground(RAYWHITE);
    p.draw();
    EndDrawing();
  }
}