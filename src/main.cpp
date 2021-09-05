#include <Arduino.h>
#include <arduino-timer.h>
#include "servoCtl.h"

#define KNOB 34
#define SERVO_PIN 23
const int MIN_ROTATION = 0;
const int MAX_ROTATION = 120;


auto timer = timer_create_default();

int knobRotaryPosition = 0;
float rad = 0;

void setup() {
    Serial.begin(9600);    
    Serial.println("Hello world");

    servoCtlInit(SERVO_PIN);

    // update rotaryPosition
    timer.every(200, [](void*) -> bool { 
      float knobValue = analogRead(KNOB); //0.00 -> 4095
      int realRotaryPosition = map(round(knobValue), 0, 4095, 0, 100); // convert raw rotation range [0..4095] to nice rotation [0..100]
      int roundedRotationPosition = round(realRotaryPosition / 5) * 5; // keep rotation in steps of 5 to negate anoying fluxuating knob value.
      if (knobRotaryPosition == roundedRotationPosition) return true; // nothing changed, do nothing
      Serial.println(String("New rotary " + String(roundedRotationPosition)));
      knobRotaryPosition = roundedRotationPosition;
      return true; 
      });


    // tick sinRad from rotaryPosition
    timer.every(20, [](void*) -> bool { 
      if (knobRotaryPosition == 0) return true; // Special case for knob==0. Just stop the rocking.
      float toAdd =  map(knobRotaryPosition, 1, 100, 2, 18) ; // high value from knob, more agressive step. Steps are [2..18]
      rad = rad + (toAdd/100);
      return true; 
      });


    // tick servo move
    timer.every(10, [](void*) -> bool { 
      float newMotorPosition = map(cos(rad)*100, -100, 100, MIN_ROTATION, MAX_ROTATION);
      servoMove(newMotorPosition);
      return true; 
      });
}


void loop() {
  timer.tick();
}