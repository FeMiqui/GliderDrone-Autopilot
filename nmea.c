#ifndef NMEA_H
#define NMEA_H
#ifndef GPMRC_H //changed
#define GPMRC_H //changed

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
  
  double  min_lon;  // minutes longitude
  double  min_lat;  // minutes latitude
  short   deg_lon;  // degrees longitude
  char    deg_lat;  // degrees latitude
  char    ns;       // north/south indicator
  char    ew;       // east/west indicator
  int     fix = 0;  // fix quality

  int temp = sscanf(s->data, 
         "%2hhd%2hhd%2hhd.%3hd,%2hhd%7lf,%c,%3hd%7lf,%c,%1d,%*2d,%*4f,%4lf", 
         &d->time.hour, &d->time.min, &d->time.sec, &d->time.msec, 
         &deg_lat, &min_lat, &ns,
         &deg_lon, &min_lon, &ew,
         &fix, &d->pos.alt);

  d->pos.coord.lat = simplifyDegrees(deg_lat, min_lat);
  d->pos.coord.lon = simplifyDegrees(deg_lon, min_lon);

  if(ns == 'S') {
    d->pos.coord.lat = -(d->pos.coord.lat);
  }

  if(ew == 'W') {
    d->pos.coord.lon = -(d->pos.coord.lon);
  }

  if(temp != 12 || !fix) { // this data is invalid
     return 1;
  }

  return 0;
}

int readRmcData(NmeaSentence* s, RmcData* d) {
  // validate input
  if(!s || !d || strcmp(s->datatype, GGA_DATATYPE_STR) || validateChecksum(s)) {
    return 1;
  }

  double speed;    // speed in knots

  int temp = sscanf(s->data, 
         "%*2d%2*d%*2d.%*3hd,%c,%*2d%*7f,%*c,%*3d%*7f,%*c,%4lf,%6lf,%*2d,%*2d,%*2d,%*4f,%*c,%c", 
         &d->status, &speed, &d->course, &d->mode);

  if(status == "V") { // this data is invalid
     return 1;
  }
  //meters per second = knots × 0.514444
  d->speed = speed*0.514444;

  return 0;
}
#endif
