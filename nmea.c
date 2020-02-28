#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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
    checksum = 'G' ^ 'P';

    for(i = 0; i < NMEA_DATATYPE_LEN; i++) {
      checksum ^= s->datatype[i];
    }

    checksum ^= ',';

    for(i = 0; i < s->data_len; i++) {
       checksum ^= s->data[i];
    }

    checksum ^= s->checksum;
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

int createNmeaSentence(const char* datatype, const char* data, NmeaSentence* s) {
  // validate input
  if(!datatype || !data || !s) {
    return 1;
  }

  strncpy(s->datatype, datatype, NMEA_DATATYPE_LEN);
  strncpy(s->data, data, NMEA_DATA_LEN);
  s->data_len = strlen(s->data); 
  createChecksum(s);
  return 0;
}

int parseNmeaSentence(const char* buffer, int len, NmeaSentence* s) {
  // validate input
  if(!buffer || !s ||  len <= 0 || len > 82 || buffer[0] != '$' 
                  || buffer[1] != 'G' || buffer[2] != 'P' 
                  || buffer[len - 2] != 0x0d || buffer[len - 1] != 0x0a) {
    return 1;
  }
  
  // copy datatype
  strncpy(s->datatype, buffer + 3, NMEA_DATATYPE_LEN);
  
  // copy data
  memset(s->data, 0, NMEA_DATA_LEN);
  strncpy(s->data, buffer + NMEA_DATATYPE_LEN + 4, len - NMEA_DATATYPE_LEN - 9);
  s->data_len = strlen(s->data);

  // copy checksum
  s->checksum = (char) strtol(buffer + len - 4, NULL, 16);
  
  return 0;
}

int readGgaData(NmeaSentence* s, GgaData* d) {
  // validate input
  if(!s || !d || strcmp(s->datatype, GGA_DATATYPE_STR) || validateChecksum(s)) {
    return 1;
  }
  
  char ns; // north/south indicator
  char ew; // east/west indicator
  int fix = 0; // fix quality

  int temp = sscanf(s->data, 
         "%2hhd%2hhd%2hhd.%3hd,%2hhd%7lf,%c,%3hd%7lf,%c,%1d,%*2d,%*4f,%4lf", 
         &d->hour, &d->min, &d->sec, &d->msec, 
         &d->latitude.deg, &d->latitude.min, &ns,
         &d->longitude.deg, &d->longitude.min, &ew,
         &fix, &d->altitude);

  if(ns == 'S') {
    d->latitude.deg = -(d->latitude.deg);
  }

  if(ew == 'W') {
    d->longitude.deg = -(d->longitude.deg);
  }

  if(temp != 12 || !fix) { // this data is invalid
     return 1;
  }

  return 0;
}
