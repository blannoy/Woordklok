#pragma once
#include <headers.h>
#define DEBUGMEM false
#define DEBUG true

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
  /*    memset(&logBufferString[0], 0, sizeof(logBufferString));
      logPointer=0;
      Log.flush();*/
}

void putchar_(char character)
{
  Serial.print(character);
  /* if (logPointer<sizeof(logBufferString)){
     logBufferString[logPointer]=character;
   }

   logPointer++;*/
}

#define reportmem(...)                                                     \
  do                                                                       \
  {                                                                        \
    if (DEBUGMEM)                                                          \
    {                                                                      \
      char stack;                                                          \
      float ratio = (float)ESP.getFreeContStack() / (float)beginFreeStack; \
      if (ratio < 0.20)                                                    \
      {                                                                    \
        Serial.print(__VA_ARGS__);                                         \
        Serial.print(F(" - "));                                            \
        Serial.print(F(" stack "));                                        \
        Serial.print(stack_start - &stack);                                \
        Serial.print(F(" - "));                                            \
        Serial.print(ESP.getFreeContStack());                              \
        Serial.print(F(" / "));                                            \
        Serial.print(beginFreeStack);                                      \
        Serial.print(F(" heap "));                                         \
        Serial.print(ESP.getFreeHeap());                                   \
        Serial.print(F(" !!!!!!WARNING!!!!!"));                            \
        Serial.print(ratio);                                               \
        Serial.println();                                                  \
      }                                                                    \
    }                                                                      \
  } while (0)

char *stack_start;
uint32_t beginFreeStack;

void initMemLog()
{
#ifdef DEBUGMEM
  char stack;
  stack_start = &stack;
  beginFreeStack = ESP.getFreeContStack();
#endif
}

void randomColor(char hexArray[8])
{
  HslColor randomHueColor = HslColor(((float)random(360)) / 360, 1, 0.5);
  RgbColor randomRgbColor = RgbColor(randomHueColor);
  snprintf(hexArray, 8, "#%02X%02X%02X", randomRgbColor.R, randomRgbColor.G, randomRgbColor.B);
}

RgbColor hexToRgb(char *hex)
{
  return hexToRgb(hex, NULL);
}

RgbColor hexToRgb(char *hex, RgbColor color)
{
  long long r;
  long long g;
  long long b;

// Complementary color
  if ((strcmp(hex,"#xxxxxx")) && (color != NULL)){
    r=255-color.R;
    g=255-color.G;
    b=255-color.B;
  } else {
    long long number = strtoll(&hex[1], NULL, 16);

    // Split them up into r, g, b values
    r = number >> 16;
    g = number >> 8 & 0xFF;
    b = number & 0xFF;
  }

return RgbColor(r,g,b);
}