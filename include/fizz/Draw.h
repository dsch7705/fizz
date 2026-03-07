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

using CircleCallback = std::function<void(double x, double y, float radius, Color color)>;
using LineCallback = std::function<void(double x0, double y0, double x1, double y1, Color color)>;

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
void circle(double x, double y, float radius, Color color);
// Note: p0 and p1 should be in world space
void line(double x0, double y0, double x1, double y1, Color color);

}  // namespace Draw