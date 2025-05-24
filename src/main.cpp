
//@todo configuration.h: JSON validation rules: clockface fully defined, Check JSON words == wordcolors; total number of referenced leds <= totalleds
//@todo ledcode.h: check if not more than maxleds
//@todo wifi.h: remove config.json logic
//@todo weather representation

#define DEBUG  false
#define DEBUGMEM false

#define LEDSTRIPPIN 13

// clock has light sensor
#define HASLDR 
#define LDRPIN A1

// clock has touch sensor
#define HASTOUCHBUTTON
#if defined(ESP32)
#define TOUCHPIN T12
#endif

#include <Arduino.h>
#include <utils.h>
#include "filesystem.h"
#include "clockface.h"
#include "configuration.h"
#include "ledcode.h"
#include <sensors.h>
#include <wifihelper.h>
#include <webserverhelper.h>
#include <timeutils.h>
#include <tests.h>

states state = BOOT;
long lastLedUpdate = 0;
long bootTime = 0;

void setup() {
  Serial.begin(115200);
  debug_begin(115200);
  initMemLog();
    debug_printf("Setup filesystem\n");
  filesystemSetup();
    debug_printf("\nConfiguration...\n");
  configurationSetup();
   debug_printf("Setup sensors\n");
  setupSensors();
  debug_printf("Setup led\n");
  ledSetup();
  filesystemSetup();
  debug_printf("Software version %s", version.c_str());
  //printConfig(config);
  debug_printf("Running...\n");
  }

void testloop(){
    timeLoop();
  webServerLoop();
  sensorLoop();
  wifiLoop();
}
void loop()
{
  timeLoop();
  webServerLoop();
  sensorLoop();
  wifiLoop();
  // if (dateYear > 70) {
  switch (state)
  {
  case BOOT:
    setStatusLeds();
    setupRain();
    wifiSetup();
    timeSetup();
    bootTime = millis();
    changeState(STARTUP);
    break;
  case STARTUP:
    if ((millis() - bootTime) < BOOTANIMTIME)
    {
      rainLoop();
    }
    else
    {
      stopRain();
      if (dateYear > 70)
      {
        lastLedUpdate = 0;
        changeState(CLOCK);
      }
    }

    break;
  case TOUCHED:
    // touchtrigger = true; // execute touch event only once
    // changeState(SPLASH);
    // break;
  case SPLASH:
    showSplash();
    changeState(CLOCK);
    break;
  case CLOCK:
    // if (touchtrigger & !touched)
    //   touchtrigger = false;
    // if (touched & !touchtrigger)
    // { // execute touch event only once
    //   changeState(TOUCHED);
    // }
    clockLoop();
    break;
  case TESTPIXEL:
    if (testPixel())
    {
      changeState(CLOCK);
    }
    break;
  case TESTFULL:
    if (testFull())
    {
      changeState(CLOCK);
    }
    break;
  case TESTHOURS:
    if (testHours())
    {
      changeState(CLOCK);
    }
    break;
  case TESTMINUTES:
    if (testMinutes())
    {
      changeState(CLOCK);
    }
    break;
  case TESTCOLORS:
    if (testStart == 0)
    {
      testStart = millis();
      debug_println("Starting test");
      //  printConfig(config);
    }
    else if (((millis() - testStart) > TESTTIMER))
    {
      debug_printf("End of test %lu\n", (millis() - testStart));
      changeState(CLOCK);
      copyConfig(config, workingConfig);
      // printConfig(config);
      testStart = 0;
    }
    else
    {
      switch (config.ledMode)
      {
      case LedMode::singleColor:
        clockLoop();
        break;
      case LedMode::rainbowColor:
        clockLoop();
        break;
      case LedMode::hourlyColor:
        testHourlyColors();
        break;
      case LedMode::wordColor:
        testWordColors();
        break;
      default:
        break;
      }
    }
    break;
  }
  //}
  /*if (touched && !splashed){
    showSplash();
    splashed=true;
    } else if (!touched && splashed){
    splashed=false;
    }*/
  logLoop();
  delay(10);
}