#pragma once

#include "DVec.h"

#include <cstdint>
#include <functional>

namespace Draw {

struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};

using CircleCallback = std::function<void(const DVec2& center, float radius, Color color)>;
using LineCallback = std::function<void(const DVec2& p0, const DVec2& p1, Color color)>;

struct Transform {
  DVec2 offset;
  double scale;
};

void setCircleCallback(CircleCallback func);
void setLineCallback(LineCallback func);

Transform& getTransform();

DVec2 worldToScreen(const DVec2& pos);
DVec2 screenToWorld(const DVec2& pos);

// Note: center and radius should be in world space
void circle(const DVec2& center, float radius, Color color);
// Note: p0 and p1 should be in world space
void line(const DVec2& p0, const DVec2& p1, Color color);

}  // namespace Draw