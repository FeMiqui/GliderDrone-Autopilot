#define GGA_DATATYPE 0

typedef struct s_NmeaSentence {
  char checksum;
  char datatype;
  char msg[73];
} NmeaSentence;

typedef struct s_GgaData {
  double latitude;
  double longitude;
  double altitude;
} GgaData;

/*
 * Creates an NMEA sentence with the given information.
 * Returns NULL on any error.
 */
NmeaSentence* createNmeaSentence(char datatype, char* msg);

/*
 * Populates the GgaData struct with information from the NMEA sentence.
 * Returns NULL on any error.
 */
GgaData* readGgaData(NmeaSentence* s);

/*
 * Frees the NMEA sentence and associated information.
 */
void freeNmeaSentence(NmeaSentence s);
