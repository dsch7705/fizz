#include "fizz/Util.h"
#include "fizz/Constants.h"

//DVec2 Util::worldToScreen(DVec2 coord)
//{
//  return coord * kPixelsPerMeter;
//}
//
//DVec2 Util::screenToWorld(DVec2 coord)
//{
//  return coord * (1.0 / kPixelsPerMeter);
//}

int Util::nextId()
{
  static int nextId = 0;
  return nextId++;
}