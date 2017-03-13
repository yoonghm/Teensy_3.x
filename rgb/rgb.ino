#include "mbed.h"
#include <math.h>
 
#ifndef M_PI
#define M_PI 3.1415
#endif

const int redPin   =  3;
const int bluePin  =  4;
const int greenPin =  5;

int redPulse, bluePulse, greenPulse;

void lit() {
  analogWrite(redPin,   redPulse);
  analogWrite(bluePin,  bluePulse);
  analogWrite(greenPin, greenPulse);
}

void setup() {
  pinMode(redPin,   OUTPUT);
  pinMode(bluePin,  OUTPUT);
  pinMode(greenPin, OUTPUT);
}

void loop() {
  // Add 1.0 to ensure all values are positive
  redPin   = sin( (f        ) * M_PI / 180.0) + 1.0;
  bluePin  = sin( (f + 180.0) * M_PI / 180.0) + 1.0;
  greenPin = sin( (f + 270.0) * M_PI / 180.0) + 1.0;
  lit();
  f += 1.0;
  wait(0.01);
}
