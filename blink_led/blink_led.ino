void blink(uint8_t pin, uint32_t count) {
  // volatile uint32_t index;

  for (; count; --count) {
    digitalWriteFast(pin, !digitalRead(pin));
    delay(100);
    digitalWriteFast(pin, !digitalRead(pin));
    delay(100);
    // for (index = 0; index < 80000; ++index) {}
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
