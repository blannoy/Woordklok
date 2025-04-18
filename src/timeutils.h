#pragma once 

#include <Arduino.h>  
#include "headers.h"                
#if defined(ESP8266)
#include <coredecls.h>                  // settimeofday_cb()
#include <TZ.h>
#define MYTZ TZ_Europe_Brussels
#endif

#if defined(ESP32)
#define MYTZ "CET-1CEST,M3.5.0,M10.5.0/3"
#endif

#include <time.h>                       // time() ctime()
#include <sys/time.h>                   // struct timeval

static time_t now;

uint8_t dateHours;
uint8_t dateMinutes;
uint8_t dateSeconds;
uint8_t dateDay;
uint8_t dateMonth;
uint8_t dateYear;

long lastTimeUpdate = 0;
long timeUpdateInterval = 20000; // 20 seconds

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time 1");
    return;
  }
  // debug_printf("%s, %s %02d %04d %02d:%02d:%02d zone %s %s\n", 
  //   &timeinfo, 
  //   timeinfo.tm_wday, 
  //   timeinfo.tm_mon, 
  //   timeinfo.tm_mday, 
  //   timeinfo.tm_year, 
  //   timeinfo.tm_hour, 
  //   timeinfo.tm_min, 
  //   timeinfo.tm_sec);

}

void timeSetup() {
  struct tm timeinfo;
  #if defined(ESP8266)
 // settimeofday_cb(time_is_set_scheduled);
  configTime(MYTZ, config.ntp_server);
  #endif
  #if defined(ESP32)
  configTime(0, 0, config.ntp_server);
  setenv("TZ",MYTZ,1);  //  Now adjust the TZ.  Clock settings are adjusted to show the new local time
  tzset();
  #endif
//  debug_printf("Setting time via %s\n",config.ntp_server);
  if(!getLocalTime(&timeinfo)){
    Serial.println("  Failed to obtain time");
    return;
  }
  Serial.println("  Got the time from NTP");
  yield();
}

void timeLoop() {
  now = time(nullptr);
  struct tm *tm = localtime(&now);
  dateMinutes = tm->tm_min;
  dateHours = tm->tm_hour;
  dateSeconds = tm->tm_sec;
  dateDay = tm->tm_mday;
  dateMonth = tm->tm_mon;
  dateYear = tm->tm_year;
  // if (showTimeNow) {
  //   showTime();
  // }
  if (millis() - lastTimeUpdate > timeUpdateInterval) {
    lastTimeUpdate = millis();
    printLocalTime();
  }
}
