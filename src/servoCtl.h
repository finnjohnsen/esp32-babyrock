// ******** second .h ******
#ifndef servoCtl_h
#define servoCtl_h
#include <ESP32Servo.h> 

void servoCtlInit(int servoPinArg);
void servoMove(int deg);

#endif