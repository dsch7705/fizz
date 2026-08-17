#pragma once

#include "Vec2.h"

#include <cstdint>
#include <functional>

namespace Draw {

struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};

using CircleCallback = std::function<void(const Vec2& center, float radius, Color color)>;
using LineCallback = std::function<void(const Vec2& p0, const Vec2& p1, float thickness, Color color)>;

struct Transform {
  Vec2 offset;
  float scale;
  Vec2 screenSize;

  // Note: point should be in world space
  void center(const Vec2& point = {0.f, 0.f});
  // Note: delta should be in screen space
  void pan(const Vec2& delta);

  void zoom(float delta);

  void resizeScreen(const Vec2& size);
};

void setCircleCallback(CircleCallback func);
void setLineCallback(LineCallback func);

Transform& getTransform();

Vec2 worldToScreen(const Vec2& pos);
Vec2 screenToWorld(const Vec2& pos);

// Note: center and radius should be in world space
void circle(const Vec2& center, float radius, Color color);
// Note: p0 and p1 should be in world space
void line(const Vec2& p0, const Vec2& p1, float thickness, Color color);

void grid(float thickness, Color color);

}  // namespace Draw