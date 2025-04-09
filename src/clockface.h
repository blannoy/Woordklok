#pragma once
#include <headers.h>

uint8_t splashScreen[] = {97, 70, 75, 80, 68, 65, 48, 53, 42, 37, 25, 19, 15, 11};
bool alwaysOnMethod(uint8_t hour, uint8_t minute)
{
  return true;
}

bool isJustMethod(uint8_t hour, uint8_t minute)
{
  return (minute == 0);
}

bool firstMinuteMethod(uint8_t hour, uint8_t minute)
{
  return (minute % 5) > 0;
}

bool secondMinuteMethod(uint8_t hour, uint8_t minute)
{
  return (minute % 5) > 1;
}

bool thirdMinuteMethod(uint8_t hour, uint8_t minute)
{
  return (minute % 5) > 2;
}

bool fourthMinuteMethod(uint8_t hour, uint8_t minute)
{
  return (minute % 5) > 3;
}

bool fiveMinuteMethod(uint8_t hour, uint8_t minute)
{
  switch (minute)
  {
  case 5:
  case 6:
  case 7:
  case 8:
  case 9:
  case 25:
  case 26:
  case 27:
  case 28:
  case 29:
  case 35:
  case 36:
  case 37:
  case 38:
  case 39:
  case 55:
  case 56:
  case 57:
  case 58:
  case 59:
    return true;
  }

  return false;
}

bool tenMinuteMethod(uint8_t hour, uint8_t minute)
{
  switch (minute)
  {
    case 10: case 11: case 12: case 13: case 14:
    case 20: case 21: case 22: case 23: case 24:
    case 40: case 41: case 42: case 43: case 44:
    case 50: case 51: case 52: case 53: case 54:
    return true;
  }

  return false;
}

bool quarterMethod(uint8_t hour, uint8_t minute)
{
  switch (minute)
  {
  case 45:
  case 46:
  case 47:
  case 48:
  case 49:
  case 15:
  case 16:
  case 17:
  case 18:
  case 19:
    return true;
  }

  return false;
}

bool pastMethod(uint8_t hour, uint8_t minute)
{
  if ((minute >= 5) && (minute < 20))
    return true;
  if ((minute >= 35) && (minute < 45))
    return true;
  return false;
}

bool beforeMethod(uint8_t hour, uint8_t minute)
{
  if ((minute >= 20) && (minute < 30))
    return true;
  if ((minute >= 45) && (minute < 60))
    return true;
  return false;
}
bool wholeHourMethod(uint8_t hour, uint8_t minute)
{
  return minute < 5;
}

bool halfHourMethod(uint8_t hour, uint8_t minute)
{
  return ((minute >= 20) && (minute < 45));
}
bool isHourActive(int8_t testHour, uint8_t hour, uint8_t minute)
{
  int8_t previousHour = testHour - 1;
  if (previousHour < 0)
    previousHour = 11;

  if (config.clockfaceLayout.hasTwenty){
  if ((minute < 25) && (testHour == hour))
    return true;
  if ((minute >= 25) && (previousHour == hour))
    return true;
  } else {
  if ((minute < 20) && (testHour == hour))
    return true;
  if ((minute >= 20) && (previousHour == hour))
    return true;
  }
  // 0 - 24 huidig uur
  // 25 - 60 vorig uur


  return false;
}
// _20 methods for clocks with twenty before/past
bool tenMinuteMethod_20(uint8_t hour, uint8_t minute)
{
  switch (minute)
  {
  case 10:
  case 11:
  case 12:
  case 13:
  case 14:
  case 50:
  case 51:
  case 52:
  case 53:
  case 54:
    return true;
  }

  return false;
}
bool twentyMethod(uint8_t hour, uint8_t minute)
{
  switch (minute)
  {
  case 20:
  case 21:
  case 22:
  case 23:
  case 24:
  case 40:
  case 41:
  case 42:
  case 43:
  case 44:
    return true;
  }

  return false;
}
bool pastMethod_20(uint8_t hour, uint8_t minute)
{
  if ((minute >= 5) && (minute < 25))
    return true;
  if ((minute >= 35) && (minute < 40))
    return true;
  return false;
}

bool beforeMethod_20(uint8_t hour, uint8_t minute)
{
  if ((minute >= 25) && (minute < 30))
    return true;
  if ((minute >= 40) && (minute < 60))
    return true;
  return false;
}
bool halfHourMethod_20(uint8_t hour, uint8_t minute)
{
  return ((minute >= 25) && (minute < 40));
}

bool isOneActiveMethod(uint8_t hour, uint8_t minute)
{
  return isHourActive(1, hour, minute);
}
bool isTwoActiveMethod(uint8_t hour, uint8_t minute)
{
  return isHourActive(2, hour, minute);
}
bool isThreeActiveMethod(uint8_t hour, uint8_t minute)
{
  return isHourActive(3, hour, minute);
}
bool isFourActiveMethod(uint8_t hour, uint8_t minute)
{
  return isHourActive(4, hour, minute);
}
bool isFiveActiveMethod(uint8_t hour, uint8_t minute)
{
  return isHourActive(5, hour, minute);
}
bool isSixActiveMethod(uint8_t hour, uint8_t minute)
{
  return isHourActive(6, hour, minute);
}
bool isSevenActiveMethod(uint8_t hour, uint8_t minute)
{
  return isHourActive(7, hour, minute);
}
bool isEightActiveMethod(uint8_t hour, uint8_t minute)
{
  return isHourActive(8, hour, minute);
}
bool isNineActiveMethod(uint8_t hour, uint8_t minute)
{
  return isHourActive(9, hour, minute);
}
bool isTenActiveMethod(uint8_t hour, uint8_t minute)
{
  return isHourActive(10, hour, minute);
}
bool isElevenActiveMethod(uint8_t hour, uint8_t minute)
{
  return isHourActive(11, hour, minute);
}
bool isTwelveActiveMethod(uint8_t hour, uint8_t minute)
{
  return isHourActive(0, hour, minute);
}



isActiveMethod methodStringToMethod(const char* method)
{

  uint8_t nrMethodStrings=sizeof(isActiveMethodStrings) / sizeof(isActiveMethodStrings[0]);
   for (uint8_t iMethod = 0; iMethod < nrMethodStrings; iMethod++)
  {
    if (strcmp(method,isActiveMethodStrings[iMethod])==0)
     {
       return (isActiveMethod)iMethod;

     };
  } 
      return isActiveMethod::alwaysOn;
}

bool isActiveCheck(isActiveMethod isActive, uint8_t hour, uint8_t minute)
{
  switch (isActive)
  {
  case isActiveMethod::alwaysOn:
    return alwaysOnMethod(hour, minute);
    break;
  case isActiveMethod::isJust:
    return isJustMethod(hour, minute);
    break;
  case isActiveMethod::firstMinute:
    return firstMinuteMethod(hour, minute);
    break;
  case isActiveMethod::secondMinute:
    return secondMinuteMethod(hour, minute);
    break;
  case isActiveMethod::thirdMinute:
    return thirdMinuteMethod(hour, minute);
    break;
  case isActiveMethod::fourthMinute:
    return fourthMinuteMethod(hour, minute);
    break;
  case isActiveMethod::fiveMinute:
    return fiveMinuteMethod(hour, minute);
    break;
  case isActiveMethod::tenMinute:
    return tenMinuteMethod(hour, minute);
    break;
  case isActiveMethod::quarter:
    return quarterMethod(hour, minute);
    break;
  case isActiveMethod::past:
    return pastMethod(hour, minute);
    break;
  case isActiveMethod::before:
    return beforeMethod(hour, minute);
    break;
  case isActiveMethod::wholeHour:
    return wholeHourMethod(hour, minute);
    break;
  case isActiveMethod::halfHour:
    return halfHourMethod(hour, minute);
    break;
      case isActiveMethod::tenMinute_20:
    return tenMinuteMethod_20(hour, minute);
    break;
      case isActiveMethod::twenty:
    return twentyMethod(hour, minute);
    break;
    case isActiveMethod::past_20:
    return pastMethod_20(hour, minute);
    break;
  case isActiveMethod::before_20:
    return beforeMethod_20(hour, minute);
    break;
  case isActiveMethod::halfHour_20:
    return halfHourMethod_20(hour, minute);
    break;  
  case isActiveMethod::isOneActive:
    return isOneActiveMethod(hour, minute);
    break;
  case isActiveMethod::isTwoActive:
    return isTwoActiveMethod(hour, minute);
    break;
  case isActiveMethod::isThreeActive:
    return isThreeActiveMethod(hour, minute);
    break;
  case isActiveMethod::isFourActive:
    return isFourActiveMethod(hour, minute);
    break;
  case isActiveMethod::isFiveActive:
    return isFiveActiveMethod(hour, minute);
    break;
  case isActiveMethod::isSixActive:
    return isSixActiveMethod(hour, minute);
    break;
  case isActiveMethod::isSevenActive:
    return isSevenActiveMethod(hour, minute);
    break;
  case isActiveMethod::isEightActive:
    return isEightActiveMethod(hour, minute);
    break;
  case isActiveMethod::isNineActive:
    return isNineActiveMethod(hour, minute);
    break;
  case isActiveMethod::isTenActive:
    return isTenActiveMethod(hour, minute);
    break;
  case isActiveMethod::isElevenActive:
    return isElevenActiveMethod(hour, minute);
    break;
  case isActiveMethod::isTwelveActive:
    return isTwelveActiveMethod(hour, minute);
    break;
  default:
    debug_println("No active method found");
    return alwaysOnMethod(hour, minute);
    break;
  }
}
/* ClockfaceWord clockface[] = {
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
 { { 110,107,103,102,97,92,80,75,70,65,54,53,48,42,37,25,20,19,15,11,10 }, 27, "kzkogremaktdzoorrbodb", alwaysOn }
};

 */
