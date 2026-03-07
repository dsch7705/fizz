#include "fizz/Draw.h"
#include "fizz/Constants.h"
#include "fizz/Util.h"

namespace Draw {

  Transform transform {
    .offset = {0.0, 0.0},
    .scale = 1.0,
  };
  Transform& getTransform()
  {
    return transform;
  }

  DVec2 worldToScreen(const DVec2& pos)
  {
    return (pos + transform.offset) * transform.scale;
  }
  DVec2 screenToWorld(const DVec2& pos)
  {
    return pos / transform.scale - transform.offset;
  }

  CircleCallback drawCircleCallback;
  void setCircleCallback(CircleCallback func)
  {
    drawCircleCallback = func;
  }
  
  LineCallback drawLineCallback;
  void setLineCallback(LineCallback func)
  {
    drawLineCallback = func;
  }
  
  void circle(const DVec2& center, float radius, Color color)
  {
    drawCircleCallback(worldToScreen(center), radius * transform.scale, color);
  }
  
  void line(const DVec2& p0, const DVec2& p1, Color color)
  {
    drawLineCallback(worldToScreen(p0), worldToScreen(p1), color);
  }
}