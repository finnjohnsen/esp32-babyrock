#include <Arduino.h>
#include <arduino-timer.h>
#include "servoCtl.h"

#define KNOB 34
#define SERVO_PIN 23
const int MIN_ROTATION = 0;
const int MAX_ROTATION = 120;


auto timer = timer_create_default();

int knobRotaryPosition = 0;
float sinRad = 0;

void setup() {
    Serial.begin(9600);    
    Serial.println("Hello world");

    servoCtlInit(SERVO_PIN);

    // update rotaryPosition
    timer.every(200, [](void*) -> bool { 
      float knobValue = analogRead(KNOB);//0.00 -> 4095
      int realRotaryPosition = map(round(knobValue), 0, 4095, 0, 100);
      int roundedRotationPosition = round(realRotaryPosition / 5) * 5;
      if (knobRotaryPosition == roundedRotationPosition) return true;
      Serial.println(String("New rotary " + String(roundedRotationPosition)));
      knobRotaryPosition = roundedRotationPosition;
      return true; 
      });


    // tick sinRad from rotaryPosition
    timer.every(20, [](void*) -> bool { 
      if (knobRotaryPosition == 0) return true;
      float toAdd =  map(knobRotaryPosition, 1, 100, 2, 18) ;
      sinRad = sinRad + (toAdd/100);
      return true; 
      });


    // tick servo move
    timer.every(10, [](void*) -> bool { 
      float currDeg = map(cos(sinRad)*100, -100, 100, MIN_ROTATION, MAX_ROTATION);
      servoMove(currDeg);
      return true; 
      });
}


void loop() {
  timer.tick();
}