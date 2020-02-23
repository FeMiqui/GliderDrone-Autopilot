#define NMEA_DATA_LEN 73
#define NMEA_DATATYPE_LEN 3
#define GGA_DATATYPE_STR "GGA"

typedef struct s_NmeaSentence {
  char checksum;
  char datatype[NMEA_DATATYPE_LEN + 1];
  char data[NMEA_DATA_LEN + 1];
  char data_len;
} NmeaSentence;

typedef struct s_GgaData {
  double altitude;
  double min_longitude;   // longitude minutes
  double min_latitude;    // latitude minutes
  short deg_longitude;    // longitude degrees
  short deg_latitude;     // latitude degrees
  short msec;             // milliseconds
  char hour;              // hour
  char min;               // minutes
  char sec;               // sec
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
