#include "distance.h"

#define NMEA_DATA_LEN 73
#define NMEA_DATATYPE_LEN 3
#define GGA_DATATYPE_STR "GGA"

typedef struct s_NmeaSentence {
  char checksum;
  char datatype[NMEA_DATATYPE_LEN + 1];
  char data[NMEA_DATA_LEN + 1];
  char data_len;
} NmeaSentence;


typedef struct s_Time {
  short msec;
  char hour;
  char min;
  char sec;
} Time;

typedef struct s_GgaData {
  Position pos;
  Time time;
} GgaData;

/*
 * Creates an NMEA sentence with the given information.
 * Returns 0 on success.
 */
int createNmeaSentence(const char* datatype, const char* data, NmeaSentence* s);

/*
 * Parses the given buffer into an NMEA sentence.
 * Returns 0 on success.
 */
int parseNmeaSentence(const char* buffer, int len, NmeaSentence* s);

/*
 * Populates the GgaData struct with information from the NMEA sentence.
 * Returns 0 on success.
 */
int readGgaData(NmeaSentence* s, GgaData* d);
