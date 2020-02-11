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
  double latitude;
  double longitude;
  double altitude;
} GgaData;

/*
 * Creates an NMEA sentence with the given information.
 * Returns NULL on any error.
 */
NmeaSentence* createNmeaSentence(const char* datatype, const char* data);

/*
 * Parses the given buffer into an NMEA sentence.
 * Returns NULL on any error.
 */
NmeaSentence* parseNmeaSentence(const char* buffer, int len);

/*
 * Populates the GgaData struct with information from the NMEA sentence.
 * Returns NULL on any error.
 */
GgaData* readGgaData(NmeaSentence* s);

/*
 * Frees the NMEA sentence and associated information.
 */
void freeNmeaSentence(NmeaSentence* s);
