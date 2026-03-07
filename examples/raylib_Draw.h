#pragma once

#include "fizz/Draw.h"

#include "raylib.h"

inline void raylib_circle(double x, double y, float radius, Draw::Color color)
{
  Color c;
  c.r = color.r;
  c.g = color.g;
  c.b = color.b;
  c.a = color.a;
  DrawCircle(x, y, radius, c);
}

inline void raylib_line(double x0, double y0, double x1, double y1, Draw::Color color)
{
  Color c;
  c.r = color.r;
  c.g = color.g;
  c.b = color.b;
  c.a = color.a;
  DrawLine(x0, y0, x1, y1, c);
}