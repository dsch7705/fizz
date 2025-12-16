#include "fizz/core/Util.h"
#include "fizz/core/Constants.h"

DVec2 Util::worldToScreen(DVec2 coord)
{
  return coord * kPixelsPerMeter;
}
DVec2 Util::screenToWorld(DVec2 coord)
{
  return coord * (1.0 / kPixelsPerMeter);
}