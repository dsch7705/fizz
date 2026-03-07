#include "fizz/Draw.h"
#include "fizz/Constants.h"
#include "fizz/Util.h"

namespace Draw {

  Transform transform;
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
  
  void circle(double x, double y, float radius, Color color)
  {
    auto center = worldToScreen({x, y});
    drawCircleCallback(center.x, center.y, radius * transform.scale, color);
  }
  
  void line(double x0, double y0, double x1, double y1, Color color)
  {
    auto p0 = worldToScreen({x0, y0});
    auto p1 = worldToScreen({x1, y1});
    drawLineCallback(p0.x, p0.y, p1.x, p1.y, color);
  }
}