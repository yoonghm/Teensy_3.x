void blink(uint8_t pin, uint32_t count) {
  volatile uint32_t index;

  for (; count; --count) {
    digitalWriteFast(pin, !digitalReadFast(pin));
    for (index = 0; index < 80000; ++index) {}
    digitalWriteFast(pin, !digitalReadFast(pin));
    for (index = 0; index < 80000; ++index) {}
  }
  Serial.println();
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(0,           OUTPUT);
}

void loop() {
  blink(LED_BUILTIN,  7);
  blink(0,           10);
}
