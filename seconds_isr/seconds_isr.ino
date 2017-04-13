volatile int status = 0;

void rtc_seconds_isr() {
	digitalWrite(LED_BUILTIN, status);
	status = !status;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  RTC_IER |= 0x10;  // Enable seconds IRQ from RTC peripheral
  NVIC_ENABLE_IRQ(IRQ_RTC_SECOND); // Enable seconds IRS function in NVIC
}

void loop() {

}
