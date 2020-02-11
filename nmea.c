#include <string.h>
#include <stdlib.h>

#include "nmea.h"

/*
 * Helper function to check checksums.
 * Returns 0 if the checksum is correct.
 */
int validateChecksum(NmeaSentence* s) {
  char checksum = -1;
 
  // validate input
  if(s) {
    int i;
    checksum = 0;
    
    for(i = 0; i < NMEA_DATATYPE_LEN; i++) {
      checksum ^= s->datatype[i];
    }

    for(i = 0; i < NMEA_DATA_LEN; i++) {
      checksum ^= s->data[i];
    }
    checksum -= s->checksum;
  }
  
  return checksum;
}

/*
 * Helper function for creating checksums.
 */
void createChecksum(NmeaSentence* s) {
  // validate input
  if(s) {
    int i;
    s->checksum = 0;
  
    for(i = 0; i < NMEA_DATATYPE_LEN; i++) {
      s->checksum ^= s->datatype[i];
    }

    for(i = 0; i < NMEA_DATA_LEN; i++) {
      s->checksum ^= s->data[i];
    }
  }
}

void parseGgaData(GgaData* d, const char* data) {
  if(d) {

  }
}

NmeaSentence* createNmeaSentence(const char* datatype, const char* data) {
  // validate input
  if(!datatype || !data) {
    return NULL;
  }

  NmeaSentence* s = (NmeaSentence*) malloc(sizeof(NmeaSentence));
  
  if(!s) {
    return NULL;
  }

  strncpy(s->datatype, datatype, NMEA_DATATYPE_LEN);
  strncpy(s->data, data, NMEA_DATA_LEN);
  s->data_len = strlen(s->data); 
  createChecksum(s);
  return s;
}

NmeaSentence* parseNmeaSentence(const char* buffer, int len) {
  // validate input
  if(!buffer || len <= 0) {
    return NULL;
  }
  return NULL;
}

GgaData* readGgaData(NmeaSentence* s) {
  // validate input
  if(!s || strcmp(s->datatype, GGA_DATATYPE_STR) || validateChecksum(s)) {
    return NULL;
  }

  GgaData* d = malloc(sizeof(GgaData));

  if(!d) {
    return NULL;
  }

  char* pos = s->data;
  int num_field = 0;
  int fix;

  for (int i = 0; i < s->data_len; i++) {
    if(s->data[i] == ',') {
      switch(num_field) {
        case 1: // latitude value field
          d->latitude = atof(pos);
          break;
        case 2: // latitude N/S field
          d->latitude = (*pos == 'S') ? -(d->latitude) : d->latitude;
          break;
        case 3: // longitude value field
          d->longitude = atof(pos);
          break;
        case 4: // longitude E/W field
          d->longitude = (*pos == 'W') ? -(d->longitude) : d->longitude;
          break;
        case 5: // fix quality field
          fix = atoi(pos);
          break;
        case 8: // altitude field
          d->altitude = atof(pos);
          break;
      }

      pos = s->data + i + 1;
      num_field++;
    }
  }

  if(!fix) { // this data is invalid
    free(d);
    d = NULL;
  }

  return d;
}

void freeNmeaSentence(NmeaSentence* s) {
  free(s);
}
