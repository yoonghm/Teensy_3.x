/*
 * This example shows how to use touchRead() found in Teensy
 * Connect a jumper wire to 16 or A2 (PTB0/LLWU_P5)
 */

#define    TRPIN   16;

int capavg; // average capacitance value returned by touchRead()

#define DELTA  0.1

void setup() {
  Serial.begin(19200);
  pinMode(LED_BUILTIN, OUTPUT);

  // Calibration: Get an average capacitance value
  int n = 5; // Take 5 samples
  long lcap = 0;
  capavg = 0;
  for (int i=0; i<n; i++) {
    lcap += touchRead(TRPIN);
  }
  capavg = (int) (lcap/n);
}

void loop() {
  int cap = touchRead(TRPIN);
/*
  Serial.print(cap);
  Serial.print("/");
  Serial.println(capavg);
*/
  if ( (cap - capavg) > (int) (DELTA * capavg)) {
    digitalWrite(LED_BUILTIN, 1); delay(200); digitalWrite(LED_BUILTIN, 0);
  }
  capavg = (int) (cap + capavg) / 2; // Moving average

  delay(200);
}