#pragma once
#include "headers.h"

#define debug_begin(...)         \
  do                             \
  {                              \
    if (DEBUG)                   \
    {                            \
      Serial.begin(__VA_ARGS__); \
      while (!Serial)            \
        ;                        \
    }                            \
  } while (0)
#define debug_print(...)         \
  do                             \
  {                              \
    if (DEBUG)                   \
      Serial.print(__VA_ARGS__); \
  } while (0)
#define debug_println(...)         \
  do                               \
  {                                \
    if (DEBUG)                     \
      Serial.println(__VA_ARGS__); \
  } while (0)
#define debug_printf(...)  \
  do                       \
  {                        \
    if (DEBUG)             \
      printf(__VA_ARGS__); \
  } while (0)
#define debug_println_wait(...)    \
  do                               \
  {                                \
    if (DEBUG)                     \
      Serial.println(__VA_ARGS__); \
    delay(2000);                   \
  } while (0)


class RunningAverage
{
private:
public:
  long total = 0;
  byte readingIndex = 0;
  long readings[numReadings];

  long getAverage(long value)
  {
    total = total - readings[readingIndex];
    readings[readingIndex] = value;
    total = total + readings[readingIndex];
    readingIndex = readingIndex + 1;
    if (readingIndex >= numReadings)
    {
      readingIndex = 0;
    }
    return total / numReadings;
  }
};

class RunningStat
{
public:
  RunningStat() : m_n(0) {}

  void Clear()
  {
    m_n = 0;
  }

  void Push(double x)
  {
    m_n++;

    // See Knuth TAOCP vol 2, 3rd edition, page 232
    if (m_n == 1)
    {
      m_oldM = m_newM = x;
      m_oldS = 0.0;
    }
    else
    {
      m_newM = m_oldM + (x - m_oldM) / m_n;
      m_newS = m_oldS + (x - m_oldM) * (x - m_newM);

      // set up for next iteration
      m_oldM = m_newM;
      m_oldS = m_newS;
    }
  }

  int NumDataValues() const
  {
    return m_n;
  }

  double Mean() const
  {
    return (m_n > 0) ? m_newM : 0.0;
  }

  double Variance() const
  {
    return ((m_n > 1) ? m_newS / (m_n - 1) : 0.0);
  }

  double StandardDeviation() const
  {
    return sqrt(Variance());
  }

private:
  int m_n;
  double m_oldM, m_newM, m_oldS, m_newS;
};

void changeState(states targetState)
{
  state = targetState;
  debug_printf("Changing state to %s\n", statesStrings[targetState]);
  flushLog();
}

void logLoop()
{
  /*  if(Log.has_overrun())
   {
     flushLog();
   }*/
}

void flushLog()
{
  // TODO
}

void putchar_(char character)
{
  Serial.print(character);
  /* if (logPointer<sizeof(logBufferString)){
     logBufferString[logPointer]=character;
   }

   logPointer++;*/
}


void randomColor(char hexArray[8])
{
  HslColor randomHueColor = HslColor(((float)random(360)) / 360, 1, 0.5);
  RgbColor randomRgbColor = RgbColor(randomHueColor);
  snprintf(hexArray, 8, "#%02X%02X%02X", randomRgbColor.R, randomRgbColor.G, randomRgbColor.B);
}

RgbColor colorDefToRgb(colorDef &targetColor)
{
  return RgbColor(targetColor.r, targetColor.g, targetColor.b);
}
RgbColor colorDefToRgb(colorDef &targetColor, RgbColor color)
{
  long long r;
  long long g;
  long long b;

// Complementary color
  if (targetColor.complementary){
    r=255-color.R;
    g=255-color.G;
    b=255-color.B;
  } else {
    r=targetColor.r;
    g=targetColor.g;
    b=targetColor.b;
  }

return RgbColor(r,g,b);
}

void hexToColorDef(const char *hex,colorDef* rgb)
{
  long long number = strtoll(&hex[1], NULL, 16);

  // Split them up into r, g, b values
  rgb->r = (uint8_t) (number >> 16);
  rgb->g = (uint8_t) (number >> 8 & 0xFF);
  rgb->b = (uint8_t) (number & 0xFF);
  rgb->complementary = false;
  }

void copyColorToJson(const colorDef& color,JsonObject& jsonColor, const char* key){
   char hexColor[8];
    if (color.complementary){
      jsonColor[key]=F("#xxxxxx");
      return;
    }
   sprintf(hexColor,"#%02X%02X%02X",color.r,color.g,color.b);
   jsonColor[key]=hexColor;
}

void copyColorToJson(const colorDef& color,JsonArray& jsonColor){
   char hexColor[8];
   sprintf(hexColor,"#%02X%02X%02X",color.r,color.g,color.b);
   jsonColor.add(hexColor);
}

RgbColor hexToRgb(const char *hex)
{
  colorDef rgb;
  hexToColorDef(hex,&rgb);
  return RgbColor(rgb.r, rgb.g, rgb.b);
}