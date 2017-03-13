volatile int status = 0;

void systick_isr() {
  systick_millis_count++;   // Original ISR code
  if (systick_millis_count % 1000 == 0) {
    digitalWrite(LED_BUILTIN, status);
    status = !status;
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  
}
