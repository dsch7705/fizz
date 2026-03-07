#pragma once

#include "fizz/Draw.h"

#include "raylib.h"

inline void raylib_circle(const DVec2& center, float radius, Draw::Color color)
{
  Color c;
  c.r = color.r;
  c.g = color.g;
  c.b = color.b;
  c.a = color.a;
  DrawCircle(center.x, center.y, radius, c);
}

inline void raylib_line(const DVec2& p0, const DVec2& p1, Draw::Color color)
{
  Color c;
  c.r = color.r;
  c.g = color.g;
  c.b = color.b;
  c.a = color.a;
  DrawLine(p0.x, p0.y, p1.x, p1.y, c);
}