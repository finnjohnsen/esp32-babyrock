#include <Arduino.h>
#include "servoCtl.h"
#include <ESP32Servo.h> 

Servo servo;
int servoPin;

void servoCtlInit(int servoPinArg) 
{
  servoPin = servoPinArg;
  Serial.println("ServoCtl init");
  servo.attach(servoPin);
}

int prevDeg = 0;
void servoMove(int deg)
{
  if (deg == prevDeg) {
    return;
  }
  servo.write(deg);
  prevDeg = deg;
}
