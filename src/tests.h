#pragma once

#include <headers.h>
int testState = 0;
int currentPixel = 0;
long timeStamp;
#define WAITTIME 1000
#define ALLWAITTIME 10000

// Turn on and off all LEDs
bool testFull() {
  switch (testState) {
    case 0:
      for (int i = 0; i < NUM_LEDS; i++) {
        setColor(i, red);
      }
      timeStamp = millis();
      testState = 1;
      break;
    case 1:
      if (millis() - timeStamp > ALLWAITTIME) {
        testState = 0;
        for (int i = 0; i < PixelCount; i++) {
          setColor(i, black);
        }
        return true;
      }
      break;
  }
  showFace(false);
  return false;
}

bool testHours() {
  debug_printf("Testing clockface hours\n");
  debug_print("--- Start ---\n");


  for (uint8_t hours = 1; hours < 13; hours++) {
    clearLEDS();
    HslColor color = HslColor(red);
    for (ClockfaceWord word : clockface) {
      if (word.isActive(hours % 12, 2)) {
        for (int pos : word.leds) {

          setColor(pos, color);
        }
      }
    }
    showFace(false);
    debug_printf("%d:02\n", hours);
    delay(1000);
  }
  debug_printf("--- End ---\n");
  clearLEDS();
  return true;
}

bool testMinutes() {
  debug_printf("Testing clockface minutes\n");
  debug_printf("--- Start ---\n");
  uint8_t currentHour = dateHours;

  for (uint8_t minutes = 0; minutes < 60; minutes++) {
    clearLEDS();
    for (ClockfaceWord word : clockface) {
      if (word.isActive(currentHour % 12, minutes)) {
        for (int pos : word.leds) {
          setColor(pos, red);
        }
      }
    }
    showFace(false);
    debug_printf("%d:%d\n", currentHour, minutes);
    delay(1000);
  }
  debug_printf("--- End ---\n");
  clearLEDS();

  return true;
}
// Turn on and off each LED
bool testPixel() {
  switch (testState) {
    case 0:
      if (millis() - timeStamp > WAITTIME) {
        setColor(currentPixel, white);
        timeStamp = millis();
        testState = 1;
      }
      break;
    case 1:
      if (millis() - timeStamp > WAITTIME) {
        testState = 0;
        setColor(currentPixel, black);
        currentPixel = (currentPixel + 1) % NUM_LEDS;
        if (currentPixel == 0) {
          return true;
        }
      }
      break;
  }
  showFace(false);
  return false;
}


