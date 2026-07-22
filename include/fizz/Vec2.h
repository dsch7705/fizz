#pragma once

#include <cmath>
#include <ostream>

struct Vec2 {
  float x;
  float y;

  inline void scale(float factor)
  {
    x *= factor;
    y *= factor;
  }
  inline static Vec2 scale(const Vec2& vec, float factor) { return {.x = vec.x * factor, .y = vec.y * factor}; }

  inline void add(const struct Vec2& other)
  {
    x += other.x;
    y += other.y;
  }
  // Adds the second vector to the first
  inline static Vec2 add(const Vec2& first, const Vec2& second)
  {
    return {.x = first.x + second.x, .y = first.y + second.y};
  }

  inline void sub(const Vec2& other)
  {
    x -= other.x;
    y -= other.y;
  }
  // Subtracts the second vector from the first
  inline static Vec2 sub(const Vec2& first, const Vec2& second)
  {
    return {.x = first.x - second.x, .y = first.y - second.y};
  }

  inline float mag() const { return std::sqrt(x * x + y * y); }
  inline static float mag(const Vec2& vec) { return vec.mag(); }

  inline void normalize() { scale(1.0 / mag()); }
  inline static Vec2 normalize(Vec2 vec)
  {
    vec.normalize();
    return vec;
  }

  inline float dot(const Vec2& other) const { return (x * other.x + y * other.y); }
  inline static float dot(const Vec2& first, const Vec2& second) { return first.dot(second); }

  inline void rotate(float theta)
  {
    float x2 = cos(theta) * x - sin(theta) * y;
    float y2 = sin(theta) * x + cos(theta) * y;

    x = x2;
    y = y2;
  }
  inline static Vec2 rotate(const Vec2& vec, float theta)
  {
    Vec2 newVec = vec;
    newVec.rotate(theta);
    return newVec;
  }

  inline void operator+=(const Vec2& other) { add(other); }
  inline void operator+=(float val)
  {
    x += val;
    y += val;
  }
  inline void operator-=(const Vec2& other) { sub(other); }
  inline void operator-=(float val) { *this += -val; }
  inline void operator*=(float factor) { scale(factor); }
  inline bool operator==(const Vec2& other) const { return (x == other.x && y == other.y); }

  inline Vec2 operator+(const Vec2& other) const { return add(*this, other); }
  inline Vec2 operator-(const Vec2& other) const { return sub(*this, other); }
  inline Vec2 operator-(void) const { return scale(*this, -1); }
  inline Vec2 operator*(float factor) const { return scale(*this, factor); }
  inline Vec2 operator/(float factor) const { return scale(*this, 1.0 / factor); }
};

inline std::ostream& operator<<(std::ostream& os, const Vec2& vec)
{
  return os << "(" << vec.x << ", " << vec.y << ')';
}
