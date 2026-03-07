#include "fizz/System.h"
#include "fizz/Draw.h"

#include <iostream>

extern "C" {
  // System
  void* system_create()
  {
    return new System;
  }

  void system_destroy(void* ptr)
  {
    auto sys = static_cast<System*>(ptr);
    delete sys;
  }

  void system_update(void* ptr, double dT)
  {
    auto sys = static_cast<System*>(ptr);
    sys->update(dT);
  }

  void system_draw(void* ptr, Draw::Color color)
  {
    auto sys = static_cast<System*>(ptr);
    sys->draw(color);
  }

  int system_create_body(void* ptr, double x, double y, double radius, bool isKinematic, double mass)
  {
    auto sys = static_cast<System*>(ptr);
    Body* b = sys->createBody({x, y}, radius, isKinematic, mass);
    return b->id();
  }

  // Draw
  using PyCircleCallback = void(*)(double x, double y, float radius, Draw::Color color);
  using PyLineCallback   = void(*)(double x0, double y0, double x1, double y1, Draw::Color color);

  void draw_set_circle_callback(PyCircleCallback py_cb)
  {
    Draw::CircleCallback cb = [py_cb](const DVec2& center, float radius, Draw::Color color) {
      py_cb(center.x, center.y, radius, color);
    };
    Draw::setCircleCallback(cb);
  }

  void draw_set_line_callback(PyLineCallback py_cb)
  {
    Draw::LineCallback cb = [py_cb](const DVec2& p0, const DVec2& p1, Draw::Color color) {
      py_cb(p0.x, p0.y, p1.x, p1.y, color);
    };
    Draw::setLineCallback(cb);
  }
}