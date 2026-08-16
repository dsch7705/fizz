#include "fizz/Draw.h"
#include "fizz/Constants.h"
#include "fizz/Util.h"

namespace Draw {

Transform transform{
    .offset = {0.0, 0.0},
    .scale = 1.0,
};
Transform& getTransform()
{
  return transform;
}

Vec2 worldToScreen(const Vec2& pos)
{
  return (pos + transform.offset) * transform.scale;
}
Vec2 screenToWorld(const Vec2& pos)
{
  return pos / transform.scale - transform.offset;
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
}  // namespace Draw