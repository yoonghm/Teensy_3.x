const int sw1 = 2;

volatile int status;

void trigger() {
  digitalWrite(LED_BUILTIN, status);
  status = !status;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(sw1, INPUT_PULLDOWN);
  attachInterrupt(sw1, trigger, RISING);  
}

void loop() {
}
