#ifndef DISTANCE_H
#define DISTANCE_H

#define EARTH_RADIUS  6371000

typedef struct s_Coord {
  double lat;
  double lon;
} Coord;

typedef struct s_Position {
  Coord coord;
  double alt;
} Position;

double simplifyDegrees(short degrees, double minutes);

double getDistance(Coord* c1, Coord* c2);

#endif
