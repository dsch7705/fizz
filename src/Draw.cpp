#include "fizz/Draw.h"
#include "fizz/Constants.h"
#include "fizz/Util.h"

Draw::CircleFunc drawCircleFunc;
void Draw::setCircleFunc(CircleFunc func)
{
  drawCircleFunc = func;
}

Draw::LineFunc drawLineFunc;
void Draw::setLineFunc(LineFunc func)
{
  drawLineFunc = func;
}

void Draw::circle(const DVec2& center, float radius, Color color)
{
  drawCircleFunc(Util::worldToScreen(center), radius * kPixelsPerMeter, color);
}

void Draw::line(const DVec2& p0, const DVec2& p1, Color color)
{
  drawLineFunc(Util::worldToScreen(p0), Util::worldToScreen(p1), color);
}