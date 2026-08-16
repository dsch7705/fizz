#include "fizz/Draw.h"
#include "fizz/System.h"

#include <iostream>

#if defined(_WIN32)
#define FIZZ_API __declspec(dllexport)
#else
#define FIZZ_API __attribute__((visibility("default")))
#endif

extern "C" {
// System
FIZZ_API void* system_create()
{
  return new System;
}

FIZZ_API void system_destroy(void* ptr)
{
  auto sys = static_cast<System*>(ptr);
  delete sys;
}

FIZZ_API void system_update(void* ptr, float dT)
{
  auto sys = static_cast<System*>(ptr);
  sys->update(dT);
}

FIZZ_API void system_draw(void* ptr, Draw::Color color)
{
  auto sys = static_cast<System*>(ptr);
  sys->draw(color);
}

FIZZ_API ID system_create_body(void* ptr, float x, float y, float radius, bool isKinematic, float mass)
{
  auto sys = static_cast<System*>(ptr);
  return sys->createBody({x, y}, radius, isKinematic, mass);
}

FIZZ_API ID system_create_distance_constraint(void* ptr, ID b0, ID b1, float width)
{
  auto sys = static_cast<System*>(ptr);
  return sys->createConstraint<DistanceConstraint>(b0, b1);
}

FIZZ_API ID system_create_spring_constraint(void* ptr, ID b0, ID b1, float k, float damping)
{
  auto sys = static_cast<System*>(ptr);
  return sys->createConstraint<SpringConstraint>(b0, b1, k, damping);
}

// Draw
using PyCircleCallback = void (*)(float x, float y, float radius, Draw::Color color);
using PyLineCallback = void (*)(float x0, float y0, float x1, float y1, Draw::Color color);

FIZZ_API void draw_set_circle_callback(PyCircleCallback py_cb)
{
  Draw::CircleCallback cb = [py_cb](const Vec2& center, float radius, Draw::Color color) {
    py_cb(center.x, center.y, radius, color);
  };
  Draw::setCircleCallback(cb);
}

FIZZ_API void draw_set_line_callback(PyLineCallback py_cb)
{
  Draw::LineCallback cb = [py_cb](const Vec2& p0, const Vec2& p1, float width, Draw::Color color) {
    py_cb(p0.x, p0.y, p1.x, p1.y, color);
  };
  Draw::setLineCallback(cb);
}

FIZZ_API Draw::Transform draw_get_transform()
{
  return Draw::getTransform();
}
FIZZ_API void draw_set_transform(Draw::Transform transform)
{
  Draw::Transform& t = Draw::getTransform();
  t.offset = transform.offset;
  t.scale = transform.scale;
}
}