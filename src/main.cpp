#include <Arduino.h>
#include <headers.h>
#include <utils.h>
#include <sensors.h>
#include <clockface.h>
#include <configuration.h>
#include <wifi.h>
#include <webserver.h>
#include <timeutils.h>
#include <filesystem.h>
#include <ledcode.h>
#include <tests.h>

states state = BOOT;
long lastLedUpdate=0;
long bootTime=0;


void setup() {
    Serial.begin(115200);
    debug_begin(115200);
  debug_printf("\nInitializing...\n");
  loadConfiguration();
  debug_printf("Setup sensors\n");
  setupSensors();
  debug_printf("Setup led\n");
  ledSetup();
  debug_printf("Setup filesystem\n");
  filesystemSetup();
  debug_printf("Software version %s", version);
  debug_printf("Running...\n");
}

void loop(){
  timeLoop();
  webServerLoop();
  sensorLoop();
  wifiLoop();
  //if (dateYear > 70) {
  switch (state) {
    case BOOT:
      setStatusLeds();
      setupRain();
      wifiSetup();
      timeSetup();
      bootTime = millis();
      changeState(STARTUP);
      break;
    case STARTUP:
      if ((millis() - bootTime) < BOOTANIMTIME) {
        rainLoop();
      } else {
        stopRain();
        if (dateYear > 70) {
          lastLedUpdate = 0;
          changeState(CLOCK);
        }
      }

      break;
    case TOUCHED:
      touchtrigger = true;  // execute touch event only once
      changeState(SPLASH);
      break;
    case SPLASH:
      showSplash();
      changeState(CLOCK);
      break;
    case CLOCK:
      if (touchtrigger & !touched) touchtrigger = false;
      if (touched & !touchtrigger) {  // execute touch event only once
        changeState(TOUCHED);
      }
      clockLoop();
      break;
    case TESTPIXEL:
      if (testPixel()) {
        changeState(CLOCK);
      }
      break;
    case TESTFULL:
      if (testFull()) {
        changeState(CLOCK);
      }
      break;
    case TESTHOURS:
      if (testHours()) {
        changeState(CLOCK);
      }
      break;
    case TESTMINUTES:
      if (testMinutes()) {
        changeState(CLOCK);
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

void oldLoop() {

  wifiLoop();
  timeLoop();
  //sensorLoop();
  webServerLoop();
  delay(100);
}