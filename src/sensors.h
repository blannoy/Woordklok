#pragma once 

#include <headers.h>
#include <CapacitiveSensor.h>
#include <utils.h>

/*
   CapitiveSense Library Demo Sketch
   Paul Badger 2008
   Uses a high value resistor e.g. 10M between send pin and receive pin
   Resistor effects sensitivity, experiment with values, 50K - 50M. Larger resistor values yield larger sensor values.
   Receive pin is the sensor pin - try different amounts of foil/metal on this pin
*/


CapacitiveSensor   capacitiveTouchSensor = CapacitiveSensor(D5, D6);       // 10M resistor between pins 5 & 6, pin 6 is sensor pin, add a wire and or foil if desired



double sigma;
double baseline;
long threshold;
long lowThreshold;
long LDRvalue = 0;
bool touched = false;
bool touchtrigger = false;
void calibrateSensor() {
  debug_printf("Calibrating");
  RunningStat rs;
  for (byte loops = 0; loops < 60; loops++) {

    rs.Push(capacitiveTouchSensor.capacitiveSensor(20));
    delay(20);
  }
  sigma = rs.StandardDeviation();
  baseline = rs.Mean();
}

RunningAverage touchSensorRA;
RunningAverage LDRSensorRA;

void setupSensors()
{
  calibrateSensor();
  threshold=baseline+sigma*8;
  lowThreshold=baseline+sigma*4;

//hardcoded
// TODO: as parameters in web interface 
  threshold=250;
  lowThreshold=100;
  
 /* debug_printf(sigma);
  debug_printf("-");
  debug_printf(baseline);
  debug_printf(" low threshold ");
  debug_printf(lowThreshold);
  debug_printf(" threshold ");
  debug_printf(threshold);*/
  //delay(1000);
}

void sensorLoop()
{
  long touchSensorValue=  touchSensorRA.getAverage(capacitiveTouchSensor.capacitiveSensor(20));
  LDRvalue = LDRSensorRA.getAverage((long) analogRead(A0));
  //Serial.println(LDRvalue);
  /*debug_printf(LDRvalue);
  debug_printf(" - ");
  debug_printf(touchSensorValue);*/
  if ((touchSensorValue > threshold) && !touched) {
    touched = true;
    debug_printf("Touched");
  } else if ((touchSensorValue < lowThreshold)  && touched) {
    touched = false;
    debug_printf("Untouched");
  }
}
