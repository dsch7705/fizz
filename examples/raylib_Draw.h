#pragma once

#include "fizz/Draw.h"

#include "raylib.h"

inline bool operator==(const Color& l, const Color& r)
{
  return l.r == r.r && l.g == r.g && l.b == r.b && l.a == r.a;
}

constexpr int texW = 128;
constexpr Rectangle srcRec{0, 0, texW, texW};

inline static const Texture& circleTex()
{
  static Texture tex = {};

  if (tex.width != texW) {
    Image img = GenImageColor(texW, texW, BLANK);
    ImageDrawCircle(&img, texW / 2, texW / 2, texW / 2 - 1, WHITE);

    tex = LoadTextureFromImage(img);
    UnloadImage(img);
  }

  return tex;
}

inline void raylib_circle(const Vec2& center, float radius, Draw::Color color)
{
  Color c{.r = color.r, .g = color.g, .b = color.b, .a = color.a};

  float diam = radius * 2.f;

  Rectangle dstRec = {center.x, center.y, diam, diam};
  Vector2 origin = {radius, radius};

  DrawTexturePro(circleTex(), srcRec, dstRec, origin, 0.f, c);
}

inline void raylib_line(const Vec2& p0, const Vec2& p1, Draw::Color color)
{
  Color c{.r = color.r, .g = color.g, .b = color.b, .a = color.a};
  DrawLine(p0.x, p0.y, p1.x, p1.y, c);
}