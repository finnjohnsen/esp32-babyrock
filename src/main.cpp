#include <Arduino.h>
#include <arduino-timer.h>
#include "servoCtl.h"


#define KNOB 34
#define SERVO_PIN 23
const int MIN_ROTATION = 90;
const int MAX_ROTATION = 180;
const int REST_ROTATION = 180;


auto timer = timer_create_default();

int currentRotary1to100 = 0;
float rad = 0;

float lastMotorPosition = 0;


void setup() {
    Serial.begin(9600);    
    Serial.println("Hello world");

    servoCtlInit(SERVO_PIN);

    // timer.every(400, [](void*) -> bool { 
    //     Serial.println(String("Rotary " + String(currentRotary1to100)));
    //     Serial.println("new Motor Position value" + String(lastMotorPosition));
    //     Serial.println("rad" + String(rad));

    //     return true;

    // });


    // update currentRotary1to100
    timer.every(200, [](void*) -> bool { 
      float knobValue = analogRead(KNOB); //0.00 -> 4095
      int knob0to100 = map(round(knobValue), 0, 4095, 0, 100); // convert raw rotation range [0..4095] to nice rotation [0..100]
      int roundedKnobRotation = round(knob0to100 / 3) * 3; // keep rotation in steps of 5 to negate anoying fluxuating knob value.
      if (currentRotary1to100 == roundedKnobRotation) return true; // nothing changed, do nothing
      currentRotary1to100 = roundedKnobRotation;
      return true; 
      });


    // tick sinRad from rotaryPosition
    timer.every(20, [](void*) -> bool { 
      if (currentRotary1to100 == 0) return true; // Special case for knob==0. Just stop the rocking.
      float toAdd =  map(currentRotary1to100, 1, 100, 2, 18) ; // high value from knob, more agressive step. Steps are [2..18]
      rad = rad + (toAdd/100);
      return true; 
      });


    // tick servo move
    timer.every(10, [](void*) -> bool { 
      if (currentRotary1to100 == 0) { 
        if (rad == 0) return true;
        rad = 0;
        servoMove(REST_ROTATION); // Special case for knob==0. Just stop the rocking.
        return true;
      }

      lastMotorPosition = map(cos(rad)*100, -100, 100, MIN_ROTATION, MAX_ROTATION);
      servoMove(lastMotorPosition);

      return true; 
      });

      //initWifi();
}


void loop() {
  timer.tick();
  //servoMove(0);
}