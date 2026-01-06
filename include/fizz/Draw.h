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

using CircleFunc = std::function<void(const DVec2& center, float radius, Color color)>;
using LineFunc = std::function<void(const DVec2& p0, const DVec2& p1, Color color)>;

void setCircleFunc(CircleFunc func);
void setLineFunc(LineFunc func);

// Note: center and radius should be in world space
void circle(const DVec2& center, float radius, Color color);
// Note: p0 and p1 should be in world space
void line(const DVec2& p0, const DVec2& p1, Color color);

}  // namespace Draw