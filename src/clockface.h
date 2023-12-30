#pragma once 
#include <headers.h>

int PixelCount = NUMWORDS;
bool alwaysOn(int hour, int minute);



int splashScreen[]={97, 70, 75, 80, 68, 65, 48, 53, 42, 37, 25, 19, 15, 11};
bool alwaysOn(int hour, int minute) {
  return true;
}

bool isJust(int hour, int minute) {
  return (minute == 0);
}

bool firstMinute(int hour, int minute) {
  return (minute % 5) > 0;
}

bool secondMinute(int hour, int minute) {
  return (minute % 5) > 1;
}

bool thirdMinute(int hour, int minute) {
  return (minute % 5) > 2;
}

bool fourthMinute(int hour, int minute) {
  return (minute % 5) > 3;
}

bool fiveMinute(int hour, int minute) {
  switch (minute) {
    case  5: case  6: case  7: case  8: case  9:
    case 25: case 26: case 27: case 28: case 29:
    case 35: case 36: case 37: case 38: case 39:
    case 55: case 56: case 57: case 58: case 59:
      return true;
  }

  return false;
}

bool tenMinute(int hour, int minute) {
  switch (minute) {
    case 10: case 11: case 12: case 13: case 14:
    case 50: case 51: case 52: case 53: case 54:
      return true;
  }

  return false;
}

bool quarter(int hour, int minute) {
  switch (minute) {
    case 45: case 46: case 47: case 48: case 49:
    case 15: case 16: case 17: case 18: case 19:
      return true;
  }

  return false;
}

bool twenty(int hour, int minute) {
  switch (minute) {
    case 20: case 21: case 22: case 23: case 24:
    case 40: case 41: case 42: case 43: case 44:
      return true;
  }

  return false;
}
bool past(int hour, int minute) {
  if ((minute >= 5) && (minute < 25)) return true;
  if ((minute >= 35) && (minute < 40)) return true;
  return false;
}

bool before(int hour, int minute) {
  if ((minute >= 25) && (minute < 30)) return true;
  if ((minute >= 40) && (minute < 60)) return true;
  return false;
}

bool wholeHour(int hour, int minute) {
  return minute < 5;
}

bool halfHour(int hour, int minute) {
  return ((minute >= 25) && (minute < 40));
}

bool isHourActive(int testHour, int hour, int minute) {
  int previousHour = testHour - 1;
  if (previousHour < 0) previousHour = 11;

  // 0 - 24 huidig uur
  // 25 - 60 vorig uur
  if ((minute < 25) && (testHour == hour)) return true;
  if ((minute >= 25) && (previousHour == hour)) return true;

  return false;
}

bool isOneActive(int hour, int minute)    {
  return isHourActive( 1, hour, minute);
}
bool isTwoActive(int hour, int minute)    {
  return isHourActive( 2, hour, minute);
}
bool isThreeActive(int hour, int minute)  {
  return isHourActive( 3, hour, minute);
}
bool isFourActive(int hour, int minute)   {
  return isHourActive( 4, hour, minute);
}
bool isFiveActive(int hour, int minute)   {
  return isHourActive( 5, hour, minute);
}
bool isSixActive(int hour, int minute)    {
  return isHourActive( 6, hour, minute);
}
bool isSevenActive(int hour, int minute)  {
  return isHourActive( 7, hour, minute);
}
bool isEightActive(int hour, int minute)  {
  return isHourActive( 8, hour, minute);
}
bool isNineActive(int hour, int minute)   {
  return isHourActive( 9, hour, minute);
}
bool isTenActive(int hour, int minute)    {
  return isHourActive(10, hour, minute);
}
bool isElevenActive(int hour, int minute) {
  return isHourActive(11, hour, minute);
}
bool isTwelveActive(int hour, int minute) {
  return isHourActive( 0, hour, minute);
}

bool hasBackground(int hour, int minute) {
  return true;
}


ClockfaceWord clockface[] = {
 { { 113,112,111 }, 0, "het", alwaysOn },
 { { 109,108 }, 1, "is", alwaysOn },
 { { 106,105,104 }, 2, "net", isJust },
 { { 98,99,100,101 }, 3, "vijf", fiveMinute },
 { { 93,94,95,96 }, 4, "tien", tenMinute },
 { { 91,90,89,88,87 }, 5, "kwart", quarter },
 { { 87,86,85,84,83,82,81 }, 6, "twintig", twenty },
 { { 76,77,78,79 }, 7, "over", past },
 { { 71,72,73,74 }, 8, "voor", before },
 { { 12,13,14 }, 9, "uur", wholeHour },
 { { 69,68,67,66 }, 10, "half", halfHour },
 { { 3 }, 11, "1", firstMinute },
 { { 2 }, 12, "2", secondMinute },
 { { 1 }, 13, "3", thirdMinute },
 { { 0 }, 14, "4", fourthMinute },
 { { 64,63,62 }, 15, "een", isOneActive },
 { { 24,23,22,21 }, 16, "twee", isTwoActive },
 { { 49,50,51,52 }, 17, "drie", isThreeActive },
 { { 55,56,57,58 }, 18, "vier", isFourActive },
 { { 41,40,39,38 }, 19, "vijf", isFiveActive },
 { { 61,60,59 }, 20, "zes", isSixActive },
 { { 47,46,45,44,43 }, 21, "zeven", isSevenActive },
 { { 26,27,28,29 }, 22, "acht", isEightActive },
 { { 32,33,34,35,36 }, 23, "negen", isNineActive },
 { { 29,30,31,32 }, 24, "tien", isTenActive },
 { { 18,17,16 }, 25, "elf", isElevenActive },
 { { 4,5,6,7,8,9 }, 26, "twaalf", isTwelveActive },
 { { 110,107,103,102,97,92,80,75,70,65,54,53,48,42,37,25,20,19,15,11,10 }, 27, "kzkogremaktdzoorrbodb", hasBackground }
};

