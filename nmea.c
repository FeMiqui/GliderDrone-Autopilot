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

  strncpy(s->datatype, buffer + 3, NMEA_DATATYPE_LEN);
  strncpy(s->data, buffer + NMEA_DATATYPE_LEN + 4, len - NMEA_DATATYPE_LEN - 9);
  s->data_len = strlen(s->data);
  s->checksum = (char) strtol(buffer + len - 4, NULL, 16);
  return 0;
}

int readGgaData(NmeaSentence* s, GgaData* d) {
  // validate input
  if(!s || !d || strcmp(s->datatype, GGA_DATATYPE_STR) || validateChecksum(s)) {
    return 1;
  }

  char* pos = s->data;
  int num_field = 0;
  int fix = 0;

  for (int i = 0; i < s->data_len; i++) {
    if(s->data[i] == ',') {
      switch(num_field) {
        case 0: { // time field
          int time = atoi(pos);
          d->sec = time % 100;
          d->min = (time / 100) % 100;
          d->hour = time / 10000;
          break;
        }
        case 1: { // latitude value field
          d->latitude = atof(pos);
          break;
        }
        case 2: { // latitude N/S field 
          if(*pos == 'S') {
            d->latitude = -d->latitude;
          }
          break;
        }
        case 3: { // longitude value field
          d->longitude = atof(pos);
          break;
        }
        case 4: { // longitude E/W field
          if(*pos == 'W') {
            d->longitude = -d->longitude;
          }
          break;
        }
        case 5: { // fix quality field
          fix = atoi(pos);
          break;
        }
        case 8: { // altitude field
          d->altitude = atof(pos);
          break;
        }
      }

      pos = s->data + i + 1;
      num_field++;
    }
  }

  if(!fix) { // this data is invalid
     return 1;
  }

  return 0;
}

