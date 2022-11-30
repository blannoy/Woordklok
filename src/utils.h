#pragma once 
#include <headers.h>

class RunningAverage 
{
  private:
  public:
    long total = 0;
    byte readingIndex = 0;
    long readings[numReadings];

    long getAverage(long value) {
      total = total - readings[readingIndex];
      readings[readingIndex] = value;
      total = total + readings[readingIndex];
      readingIndex = readingIndex + 1;
      if (readingIndex >= numReadings) {
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
      return ( (m_n > 1) ? m_newS / (m_n - 1) : 0.0 );
    }

    double StandardDeviation() const
    {
      return sqrt( Variance() );
    }

  private:
    int m_n;
    double m_oldM, m_newM, m_oldS, m_newS;
};

void changeState(states targetState){
  state=targetState;
  debug_printf("Changing state to %s\n",statesStrings[targetState]);
  flushLog();
}

void logLoop(){
 /*  if(Log.has_overrun())
  {
    flushLog();
  }*/
}

void flushLog(){
//TODO
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
