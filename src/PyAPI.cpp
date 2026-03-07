#include "fizz/System.h"
#include "fizz/Draw.h"

extern "C" {
  // System
  void* create_system()
  {
    return new System;
  }

  // Draw
  void set_draw_circle_callback(void(*)())
  {

  }
}