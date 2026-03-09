#pragma once

#include <ostream>
#include <cmath>

struct DVec2 {
  double x;
  double y;

  inline void scale(double factor)
  {
    x *= factor;
    y *= factor;
  }
  inline static DVec2 scale(const DVec2& vec, double factor) { return {.x = vec.x * factor, .y = vec.y * factor}; }

  inline void add(const struct DVec2& other)
  {
    x += other.x;
    y += other.y;
  }
  // Adds the second vector to the first
  inline static DVec2 add(const DVec2& first, const DVec2& second) { return {.x = first.x + second.x, .y = first.y + second.y}; }

  inline void sub(const DVec2& other)
  {
    x -= other.x;
    y -= other.y;
  }
  // Subtracts the second vector from the first
  inline static DVec2 sub(const DVec2& first, const DVec2& second) { return {.x = first.x - second.x, .y = first.y - second.y}; }

  inline double mag() const { return std::sqrt(x*x + y*y); }
  inline static double mag(const DVec2& vec) { return vec.mag(); }

  inline void normalize() { scale(1.0 / mag()); }
  inline static DVec2 normalize(const DVec2& vec)
  {
    DVec2 norm(vec);
    norm.normalize();
    return norm;
  }

  inline double dot(const DVec2& other) const { return (x * other.x + y * other.y); }
  inline static double dot(const DVec2& first, const DVec2& second) { return first.dot(second); }

  inline void rotate(double theta)
  {
    double x2 = cos(theta) * x - sin(theta) * y;
    double y2 = sin(theta) * x + cos(theta) * y;

    x = x2;
    y = y2;
  }
  inline static DVec2 rotate(const DVec2& vec, double theta)
  {
    DVec2 newVec = vec;
    newVec.rotate(theta);
    return newVec;
  }

  inline void operator+=(const DVec2& other) { add(other); }
  inline void operator-=(const DVec2& other) { sub(other); }
  inline void operator*=(double factor) { scale(factor); }
  inline bool operator==(const DVec2& other) const { return (x == other.x && y == other.y); }

  inline DVec2 operator+(const DVec2& other) const { return add(*this, other); }
  inline DVec2 operator-(const DVec2& other) const { return sub(*this, other); }
  inline DVec2 operator-(void) const { return scale(*this, -1); }
  inline DVec2 operator*(double factor) const { return scale(*this, factor); }
  inline DVec2 operator/(double factor) const { return scale(*this, 1.0 / factor); }
};

inline std::ostream& operator<<(std::ostream& os, const DVec2& vec) { return os << "(" << vec.x << ", " << vec.y << ')'; }
