#include "nmea.h"

/*
 * Returns the most recent NMEA message sent by the GPS.
 * Return NULL on any error.
 */
NmeaSentence* getCurGpsInfo(void);
