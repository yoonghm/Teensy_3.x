// http://tiny.cc/rgb123

#include <math.h>
 
#ifndef M_PI
#define M_PI 3.1415
#endif

const int redPin   =  3;
const int greenPin =  4;
const int bluePin  =  5;
const int sw1      =  2;
const int ledPin   = 13;

float f = 0.0;
volatile int status;

int redPulse, bluePulse, greenPulse;

void trigger() {
  NVIC_DISABLE_IRQ(IRQ_PORTE);
  digitalWrite(ledPin, status);
  status = !status;
  for (int i = 0; i < 10000; i++)
  NVIC_ENABLE_IRQ(IRQ_PORTE);
}

void lit() {
  analogWrite(redPin,   redPulse);
  analogWrite(bluePin,  bluePulse);
  analogWrite(greenPin, greenPulse);
}

void setup() {
  pinMode(redPin,   OUTPUT);
  pinMode(bluePin,  OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(ledPin,   OUTPUT);  
  pinMode(sw1,      INPUT_PULLUP);
  status = 1;
  attachInterrupt(sw1, trigger, FALLING);  
}

void loop() {
  // Add 1.0 to ensure all values are positive
  redPulse   = 127*(sin((f      )*M_PI/180.0)+1.0);
  bluePulse  = 127*(sin((f+180.0)*M_PI/180.0)+1.0);
  greenPulse = 127*(sin((f+270.0)*M_PI/180.0)+1.0);
  lit();
  f += 1.0;
  delay(1);
}
