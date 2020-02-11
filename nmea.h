#define NMEA_MSG_LEN 73
#define NMEA_DATATYPE_LEN 5
#define GGA_DATATYPE_STR "GPGGA"

typedef struct s_NmeaSentence {
  char checksum;
  char datatype[NMEA_DATATYPE_LEN + 1];
  char msg[NMEA_MSG_LEN + 1];
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
NmeaSentence* createNmeaSentence(char* datatype, char* msg);

/*
 * Parses the given buffer into an NMEA sentence.
 * Returns NULL on any error.
 */
NmeaSentence* parseNmeaSentence(char* buffer, int len);

/*
 * Populates the GgaData struct with information from the NMEA sentence.
 * Returns NULL on any error.
 */
GgaData* readGgaData(NmeaSentence* s);

/*
 * Frees the NMEA sentence and associated information.
 */
void freeNmeaSentence(NmeaSentence* s);
