#pragma once 

#include <headers.h>

#include <coredecls.h>                  // settimeofday_cb()
#include <PolledTimeout.h>
#include <time.h>                       // time() ctime()
#include <sys/time.h>                   // struct timeval

extern "C" int clock_gettime(clockid_t unused, struct timespec *tp);
#include <TZ.h>
#define MYTZ TZ_Europe_Brussels
#define SECS_YR_2000  (946684800UL) // the time at the start of y2k

static time_t now;

uint8_t dateHours;
uint8_t dateMinutes;
uint8_t dateSeconds;
uint8_t dateDay;
uint8_t dateMonth;
uint8_t dateYear;

static esp8266::polledTimeout::periodicMs showTimeNow(20000);


void showTime() {   // This function is used to print stuff to the serial port, it's not mandatory
  now = time(nullptr);      // Updates the 'now' variable to the current time value

  // human readable
  
  //debug_printf("ctime:   %s \n",ctime(&now));
  /*// Here is one example showing how you can get the current month
  debug_printf("current month: ");
  debug_printf(localtime(&now)->tm_mon);
  // Here is another example showing how you can get the current year*/
  //debug_printf("current year: %d",localtime(&now)->tm_year);
}

void time_is_set_scheduled() {    // This function is set as the callback when time data is retrieved
  // In this case we will print the new time to serial port, so the user can see it change (from 1970)
  showTime();
  //TODOstatusLed(Time,green);
  //TODOLog.flush();
  //delay(1000);
}

void timeSetup() {
  settimeofday_cb(time_is_set_scheduled);
  configTime(MYTZ, config.ntp_server);
  debug_printf("Setting time via %s\n",config.ntp_server);
  yield();

  showTime();
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
  if (showTimeNow) {
    showTime();
  }
}
