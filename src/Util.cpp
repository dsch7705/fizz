#include "fizz/Util.h"
#include "fizz/Constants.h"

// Vec2 Util::worldToScreen(Vec2 coord)
//{
//   return coord * kPixelsPerMeter;
// }
//
// Vec2 Util::screenToWorld(Vec2 coord)
//{
//   return coord * (1.0 / kPixelsPerMeter);
// }

int Util::nextId()
{
  static int nextId = 0;
  return nextId++;
}