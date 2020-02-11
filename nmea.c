#include <string.h>
#include <stdlib.h>

#include "nmea.h"

/*
 * Helper function to check checksums.
 * Returns 0 if the checksum is correct.
 */
int validateChecksum(NmeaSentence* s) {
  char checksum = 0;
  int i;
  if(s) { 
    for(i = 0; i < NMEA_DATATYPE_LEN; i++) {
      checksum ^= s->datatype[i];
    }

    for(i = 0; i < NMEA_MSG_LEN; i++) {
      checksum ^= s->msg[i];
    }
    checksum -= s->checksum;
  }
  return checksum;
}

/*
 * Helper function for creating checksums.
 */
void createChecksum(NmeaSentence* s) {
  int i;

  if(s) {
    s->checksum = 0;
  
    for(i = 0; i < NMEA_DATATYPE_LEN; i++) {
      s->checksum ^= s->datatype[i];
    }

    for(i = 0; i < NMEA_MSG_LEN; i++) {
      s->checksum ^= s->msg[i];
    }
  }
}

NmeaSentence* creaeNmeaSentence(char* datatype, char* msg) {
  if(!datatype || !msg) {
    return NULL;
  }

  NmeaSentence* s = (NmeaSentence*) malloc(sizeof(NmeaSentence));
  
  if(!s) {
    return NULL;
  }

  strncpy(s->datatype, datatype, NMEA_DATATYPE_LEN);
  strncpy(s->msg, msg, NMEA_MSG_LEN);
  createChecksum(s);
  return s;
}

NmeaSentence* parseNmeaSentence(char* buffer, int len) {
  return NULL;
}

GgaData* readGgaData(NmeaSentence* s) {
  return NULL;
}

void freeNmeaSentence(NmeaSentence* s) {
  free(s);
}
