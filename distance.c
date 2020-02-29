#include <math.h>
#include "distance.h"

static double degToRadians(double n) {
  return n * M_PI / 180;
}

double simplifyDegrees(short degrees, double minutes) {
  double ret = degrees;
  ret += minutes / 60.0;
  return ret;
}

double getDistance(Coord* c1, Coord* c2) {
  double sinLat = sin(degToRadians((c2->lat - c1->lat)) / 2.0);
  double sinLon = sin(degToRadians((c2->lon - c1->lon)) / 2.0);
  double cosLat = cos(degToRadians(c1->lat)) * cos(degToRadians(c2->lat));
  double radicand = sinLat * sinLat + cosLat * sinLon * sinLon;
  return 2 * EARTH_RADIUS * asin(sqrt(fabs(radicand)));
}
