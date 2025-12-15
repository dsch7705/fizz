#include "util.h"
#include "constants.h"

DVec2 worldToScreen(DVec2 coord)
{
  return coord * kPixelsPerMeter;
}
DVec2 screenToWorld(DVec2 coord)
{
  return coord * (1.0 / kPixelsPerMeter);
}