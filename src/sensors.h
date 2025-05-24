#pragma once

#include <headers.h>
#include <utils.h>

#ifdef HASTOUCHBUTTON
#include <OneButton.h>
OneButton *button;

void fClicked(void *oneButton)
{
  debug_println("Clicked");
}

static void fDoubleClicked(void *oneButton)
{
  OneButton *button = (OneButton *)oneButton;
  debug_println("Double Clicked");
  changeState(states::SPLASH);
}

static void fLongpressStart(void *oneButton)
{
  OneButton *button = (OneButton *)oneButton;
  debug_println("Long Press Start");
}
#endif

 
#ifdef HASLDR
long LDRvalue = 0;
RunningAverage LDRSensorRA;
#endif
void setupSensors()
{
#ifdef HASTOUCHBUTTON
  debug_println("Setting up touch button");
  button = new OneButton();
  button->attachClick(fClicked, &button);
  button->attachDoubleClick(fDoubleClicked, &button);
  button->attachLongPressStart(fLongpressStart, &button);
#endif
#ifdef HASLDR
#if defined(ESP32)
  // set resolution to 10 bits (as for ESP8266)
  analogReadResolution(10);
  // set attenuation to 11db (3.3V)
  analogSetAttenuation(ADC_11db);
#endif
#endif
}

long previoustouchtimer = 0;
long touchinterval = 200;
void sensorLoop()
{
#ifdef HASLDR
  LDRvalue = LDRSensorRA.getAverage((long)analogRead(LDRPIN));
#endif
#ifdef HASTOUCHBUTTON

  bool isPressed = (touchRead(TOUCHPIN) > config.touchThreshold) ? true : false;

  // if (millis() - previoustouchtimer > touchinterval)
  // {
  //   previoustouchtimer = millis();
  //   //debug_printf("%d;%d\n", touchRead(TOUCHPIN), LDRvalue);
  //   // changeState(states::TOUCHED);
  // }

  button->tick(isPressed);
#endif
}

uint32_t readSensor(const char *sensorArg)
{
  if (strcmp(sensorArg, "touch") == 0)
  {
    return touchRead(TOUCHPIN);
  }
  else if (strcmp(sensorArg, "ldr") == 0)
  {
    return LDRvalue;
  }
  else
  {
    debug_println("Unknown sensor");
    return -1;
  }
}
