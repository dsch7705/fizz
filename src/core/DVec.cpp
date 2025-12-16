#include "fizz/core/DVec.h"

#include <cmath>

void DVec2::scale(double factor)
{
  x *= factor;
  y *= factor;
}
DVec2 DVec2::scale(const DVec2& vec, double factor)
{
  return {.x = vec.x * factor, .y = vec.y * factor};
}
void DVec2::add(const DVec2& other)
{
  x += other.x;
  y += other.y;
}
DVec2 DVec2::add(const DVec2& first, const DVec2& second)
{
  return {.x = first.x + second.x, .y = first.y + second.y};
}
void DVec2::sub(const DVec2& other)
{
  x -= other.x;
  y -= other.y;
}
DVec2 DVec2::sub(const DVec2& first, const DVec2& second)
{
  return {.x = first.x - second.x, .y = first.y - second.y};
}

double DVec2::mag() const
{
  return std::hypot(x, y);
}
double DVec2::mag(const DVec2& vec)
{
  return vec.mag();
}
void DVec2::normalize()
{
  scale(1.0 / mag());
}
DVec2 DVec2::normalize(const DVec2& vec)
{
  DVec2 norm(vec);
  norm.normalize();
  return norm;
}
double DVec2::dot(const DVec2& other) const
{
  return (x * other.x + y * other.y);
}
double DVec2::dot(const DVec2& first, const DVec2& second)
{
  return first.dot(second);
}
void DVec2::rotate(double theta)
{
  double x2 = cos(theta) * x - sin(theta) * y;
  double y2 = sin(theta) * x + cos(theta) * y;

  x = x2;
  y = y2;
}
DVec2 DVec2::rotate(const DVec2& vec, double theta)
{
  DVec2 newVec = vec;
  newVec.rotate(theta);
  return newVec;
}

std::ostream& operator<<(std::ostream& os, const DVec2& vec)
{
  return os << "(" << vec.x << ", " << vec.y << ')';
}