/*
 *  When touching pin 16 (or via jumper wire), onboard LED (pin 13) will blink
 */

int capavg = 0; // Running average capacitance

const float sensitivity = 0.1;
const int ledPin = 13;
const int touchPin = 16;

void setup()   {                
  pinMode(ledPin, OUTPUT); // onboard LED

  // Self calibration: read 5 samples, obtain average
  long lcap = 0;
  for (int i=0; i<5; i++) 
    lcap += touchRead(touchPin); // Accumulate capacitance from 5 samples
  capavg = (int) (lcap/n); // Obtain average value
}

void loop()  {
  int cap = touchRead(touchPin);

  if ( (cap-capavg) > (int) (sensitivity*capavg)) {
    digitalWrite(ledPin, 1); delay(200);
    digitalWrite(ledPin, 0); delay(200);
  }
  capavg = (int) (cap+capavg)/2; // New moving average
  delay(200);
}
