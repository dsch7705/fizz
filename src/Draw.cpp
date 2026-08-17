#include "fizz/Draw.h"
#include "fizz/Constants.h"
#include "fizz/Util.h"

#include <print>

namespace Draw {

Transform transform{
    .offset = {0.0, 0.0},
    .scale = 1.0,
};
Transform& getTransform()
{
  return transform;
}

void Transform::center(const Vec2& point)
{
  Vec2 p = {-point.x, point.y};
  offset = screenSize / 2.f / scale + p;
}

void Transform::pan(const Vec2& delta)
{
  offset += delta / scale;
}

void Transform::zoom(float delta)
{
  Vec2 p_center = screenToWorld(screenSize / 2.f);
  scale = std::max(.1f, scale + (delta * scale * 0.1f));
  center(p_center);
}

void Transform::resizeScreen(const Vec2& size)
{
  Vec2 p_center = screenToWorld(screenSize / 2.f);
  screenSize = size;
  center(p_center);
}

Vec2 worldToScreen(const Vec2& pos)
{
  Vec2 p{pos.x, -pos.y};
  return (p + transform.offset) * transform.scale;
}
Vec2 screenToWorld(const Vec2& pos)
{
  Vec2 p = pos / transform.scale - transform.offset;
  p.y *= -1.f;
  return p;
}

CircleCallback drawCircleCallback;
void setCircleCallback(CircleCallback func)
{
  drawCircleCallback = func;
}

LineCallback drawLineCallback;
void setLineCallback(LineCallback func)
{
  drawLineCallback = func;
}

void circle(const Vec2& center, float radius, Color color)
{
  drawCircleCallback(worldToScreen(center), radius * transform.scale, color);
}

void line(const Vec2& p0, const Vec2& p1, float thickness, Color color)
{
  drawLineCallback(worldToScreen(p0), worldToScreen(p1), thickness, color);
}

void grid(float thickness, Color color)
{
  constexpr int size = 100;
  for (auto i = -size; i <= size; i++) {
    Color col = color;
    if (i == 0) {
      col.a = 255;
    }
    if (i % 5 == 0) {
      col.a = std::min(255.f, col.a * 2.5f);
    }

    line({static_cast<float>(i), -size}, {static_cast<float>(i), size}, thickness, col);
    line({-size, static_cast<float>(i)}, {size, static_cast<float>(i)}, thickness, col);
  }
}

}  // namespace Draw