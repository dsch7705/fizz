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

  CircleFunc drawCircleFunc;
  void setCircleFunc(CircleFunc func)
  {
    drawCircleFunc = func;
  }
  
  LineFunc drawLineFunc;
  void setLineFunc(LineFunc func)
  {
    drawLineFunc = func;
  }
  
  void circle(const DVec2& center, float radius, Color color)
  {
    drawCircleFunc(worldToScreen(center), radius * transform.scale, color);
  }
  
  void line(const DVec2& p0, const DVec2& p1, Color color)
  {
    drawLineFunc(worldToScreen(p0), worldToScreen(p1), color);
  }
}