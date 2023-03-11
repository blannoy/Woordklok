#pragma once 

#include <headers.h>
#include <utils.h>
#include <Wire.h>
#include "Adafruit_MPR121.h"


#define INTERRUPTPIN D3
#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

long LDRvalue = 0;
bool touched = false;
bool touchtrigger = false;

RunningAverage LDRSensorRA;
void IRAM_ATTR ISR_switch()
{
  touchtrigger = true;
 // interrupts();
}

void setupSensors()
{
cap.begin(0x5A);
  delay(1000);
  if (!cap.begin(0x5A))
  {
    Serial.println("MPR121 not found, check wiring?");
  } else {
    Serial.println("MPR121 found!");
      cap.setThresholds(20, 20);
  pinMode(INTERRUPTPIN, INPUT_PULLUP);
  attachInterrupt(INTERRUPTPIN, ISR_switch, CHANGE);
  }

}

void sensorLoop()
{
  LDRvalue = LDRSensorRA.getAverage((long) analogRead(A0));
  //Serial.println(LDRvalue);
  /*debug_printf(LDRvalue);
  debug_printf(" - ");
  debug_printf(touchSensorValue);*/
    if (touchtrigger)
  {
   // Serial.println("Interrupt");
    currtouched = cap.touched();

    if ((currtouched & _BV(5)) && !(lasttouched & _BV(5)))
    {
     // Serial.println(" touched");
          touched = true;
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(5)) && (lasttouched & _BV(5)))
    {
    touched = false;
    //  Serial.println(" released");
    }
    touchtrigger = false;
    lasttouched = currtouched;
    //Serial.println(LDRvalue);
  }
}
