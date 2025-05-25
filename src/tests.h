#pragma once

#include "headers.h"
uint8_t testState = 0;
uint8_t currentPixel = 0;
long timeStamp;
#define WAITTIME 1000
#define ALLWAITTIME 10000

// Turn on and off all LEDs
bool testFull()
{
  switch (testState)
  {
  case 0:
    for (int i = 0; i < config.clockfaceLayout.totalLeds; i++)
    {
      setColor(i, red);
    }
    timeStamp = millis();
    testState = 1;
    break;
  case 1:
    if (millis() - timeStamp > ALLWAITTIME)
    {
      testState = 0;
      for (int i = 0; i < config.clockfaceLayout.totalLeds; i++)
      {
        setColor(i, black);
      }
      return true;
    }
    break;
  }
  showFace(false);
  return false;
}

bool testHours()
{
  debug_printf("Testing clockface hours\n");
  debug_print("--- Start ---\n");

  for (uint8_t hours = 1; hours < 13; hours++)
  {
    clearLEDS();
    HslColor color = HslColor(red);
    for (uint8_t iWord = 0; iWord < config.clockfaceLayout.totalWords + 1; iWord++)
    {
      ClockfaceWord word = config.clockface[iWord];
      if (isActiveCheck(word.isActive, hours % 12, 2))
      {
        for (uint8_t pos = 0; pos < strlen(word.label); pos++)
        {
          setColor(word.leds[pos], color);
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

bool testMinutes()
{
  debug_printf("Testing clockface minutes\n");
  debug_printf("--- Start ---\n");
  uint8_t currentHour = dateHours;
  strip.ClearTo(black);
  strip.Show();
  clearLEDS();
  delay(1000);
  for (uint8_t minutes = 0; minutes < 60; minutes++)
  {
    HslColor color = HslColor(red);
    for (uint8_t iWord = 0; iWord < config.clockfaceLayout.totalWords + 1; iWord++)
    {
      ClockfaceWord word = config.clockface[iWord];
      if (isActiveCheck(word.isActive, currentHour % 12, minutes))
      {
        for (uint8_t pos = 0; pos < strlen(word.label); pos++)
        {
          setColor(word.leds[pos], color);
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
bool testPixel()
{
  switch (testState)
  {
  case 0:
    if (millis() - timeStamp > WAITTIME)
    {
      setColor(currentPixel, white);
      timeStamp = millis();
      testState = 1;
    }
    break;
  case 1:
    if (millis() - timeStamp > WAITTIME)
    {
      testState = 0;
      setColor(currentPixel, black);
      currentPixel = (currentPixel + 1) % config.clockfaceLayout.totalLeds;
      if (currentPixel == 0)
      {
        return true;
      }
    }
    break;
  }
  showFace(false);
  return false;
}

bool testWordColors()
{
  // Show all words with color at brightness 30
  //clearLEDS();
    clearLEDTo(targetKlokImage, black);
  for (uint8_t iWord = 0; iWord < config.clockfaceLayout.totalWords + 1; iWord++)
  {
    ClockfaceWord word = config.clockface[iWord];
    if (word.colorCodeInTable == config.clockfaceLayout.totalWords)
    {

      for (uint8_t pos = 0; pos < strlen(word.label); pos++)
      {
        setColor(word.leds[pos], (colorDefToRgb(config.wordColor.backgroundColor)).Dim(100));
      }
    }
    else
    {
      for (uint8_t pos = 0; pos < strlen(word.label); pos++)
      {
        setColor(word.leds[pos], colorDefToRgb(config.wordColor.color[word.colorCodeInTable]));
      }
    }
  }
  showFace(false);

  clearLEDS();
  return true;
}

bool testHourlyColors()
{
  debug_printf("Testing clockface hourly\n");
  debug_print("--- Start ---\n");

  for (uint8_t hours = 1; hours < 25; hours++)
  {
    clearLEDTo(targetKlokImage, black);
    RgbColor color = colorDefToRgb(config.hourlyColor.color[hours - 1]);
    RgbColor backgroundColor = colorDefToRgb(config.hourlyColor.backgroundColor).Dim(BACKGROUNDDIMFACTOR);
    for (uint8_t iWord = 0; iWord < config.clockfaceLayout.totalWords + 1; iWord++)
    {
      ClockfaceWord word = config.clockface[iWord];
      if (word.colorCodeInTable == config.clockfaceLayout.totalWords)
      {

        for (uint8_t pos = 0; pos < strlen(word.label); pos++)
        {
          setColor(word.leds[pos], backgroundColor);
        }
      }
      else
      {
        if (isActiveCheck(word.isActive, hours % 12, 20))
        {

          for (uint8_t pos = 0; pos < strlen(word.label); pos++)
          {
            setColor(word.leds[pos], color);
          }
        }
        else
        {

          for (uint8_t pos = 0; pos < strlen(word.label); pos++)
          {
                                if (getColor(targetKlokImage,word.leds[pos])==black){
            setColor(word.leds[pos], backgroundColor);
                                }
          }
        }
      }
    }
    //    strip.Show();
    //    showFaceSimple();
    showFace(false);
    debug_printf("%d:%d\n", hours, 20);
    delay(1000);
  }
  debug_printf("--- End ---\n");
  clearLEDS();
  return true;
}