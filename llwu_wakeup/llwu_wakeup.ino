// Teensy 3.x is woken up from sleep
// RTC alarm             - LLWU_M5IF
// External pin 2 (PTD0) - LLWU_P12

// CPU speed: 2 MHz
// Note:
// Interrupt wakes up or interrupts MCU.
// To enable interrupt to do something, then
//   Enable the interrupt via NVIC_ENABLE_IRQ(IRQ_NUMBER_t IRQn)
//   Overwrite the interrupt service routine xxxx_isr()


#define EXT_LED  0
#define EXT_PIN  2   // PTD0 - LLWU_P12

void blink(uint8_t pin, uint32_t delay_ms, uint32_t count) {
  for (; count; --count) {
    digitalWriteFast(pin, !digitalReadFast(pin));
    delay(delay_ms);
    digitalWriteFast(pin, !digitalReadFast(pin));
    delay(delay_ms);
  }
}

static void llwu_isr(void)
{
#if 0
  // Disable wakeup interrupt
#if defined (HAS_KINETIS_LLWU_32CH)
  LLWU_MF5 |= LLWU_ME_WUME5;
#elif defined (HAS_KINETIS_LLWU_16CH)
  LLWU_F3 |= LLWU_ME_WUME5;
#endif
#endif
}

void rtc_alarm_isr() {
  blink(EXT_LED, 1000, 2);
}

void ext_pin2_isr() {
//LLWU_PF2 &= ~LLWU_PF2_WUF12; // Clear the flag from LLWU
  PORTD_ISFR = 1; // Clear the interrupt flag
  blink(EXT_LED, 2000, 2);
}

// Enable RTC module and 32 kHz oscillator
void rtc_alarm_setup(uint32_t nsec) {
  SIM_SCGC6 |= SIM_SCGC6_RTC; // Enable access & interrupts to RTC module
  RTC_CR |= RTC_CR_OSCE; // Enable 32 kHz oscillator

  RTC_TAR = RTC_TSR + nsec; // Clear RTC_SR_TAF
  RTC_IER |= RTC_IER_TAIE; // Enable rtc time alarm interrupt

  attachInterruptVector(IRQ_RTC_ALARM, rtc_alarm_isr);
  NVIC_SET_PRIORITY(IRQ_RTC_ALARM, 2*16); // 3rd highest priority
  // Exit from LLWU via RTC alarm, so should not clear pending signal
  // NVIC_CLEAR_PENDING(IRQ_RTC_ALARM);
  NVIC_ENABLE_IRQ(IRQ_RTC_ALARM); // Serve rtc time alarm interrupt queue
}

void external_pin_setup()
{
  SIM_SCGC5 |= SIM_SCGC5_PORTD;  // Enable clock gating for PORTD
  RTC_IER |= RTC_IER_WPON; // Enable wakeup pin
//attachInterruptVector(IRQ_PORTD, ext_pin2_isr);
  NVIC_SET_PRIORITY(IRQ_PORTD, 2*16); // 3rd highest priority
  NVIC_CLEAR_PENDING(IRQ_PORTD);
  NVIC_IS_ENABLED(IRQ_PORTD); // pin 2 is in PORTD
}

void llwu_setup(void)
{
  // Setup the required external wakeup input pins
  LLWU_PE1 = 0;
  LLWU_PE2 = 0;
  LLWU_PE3 = 0;
  LLWU_PE4 |= LLWU_PE4_WUPE12(LLWU_PE_WUPE_PIN_FALLING);  // Pin 2, detect falling edge
#if defined(HAS_KINETIS_LLWU_32CH)
  LLWU_PE5 = 0;
  LLWU_PE6 = 0;
  LLWU_PE7 = 0;
  LLWU_PE8 = 0;
#endif
  LLWU_ME  = LLWU_ME_WUME5;  // RTC alarm

  attachInterruptVector(IRQ_LLWU, llwu_isr);
  NVIC_SET_PRIORITY(IRQ_LLWU, 2*16); // 3rd highest priority
  NVIC_CLEAR_PENDING(IRQ_LLWU);
  NVIC_ENABLE_IRQ(IRQ_LLWU);
}

void __attribute__((optimize("O0"))) goSleep(void) {
  // Make sure clock monitor is off so we don't get spurious reset
  // currently not set by anything I know, so the clock monitor is not set from reset
  MCG_C6 &= ~MCG_C6_CME0;
  //
  // Write to PMPROT to allow all possible power modes
  SMC_PMPROT = SMC_PMPROT_AVLLS;// Not needed already taken care of in here.
  // VLLSx
  SMC_PMCTRL |= SMC_PMCTRL_STOPM(0x4);

  SMC_VLLSCTRL =  SMC_VLLSCTRL_VLLSM(0x3); // VLLS3
  // wait for write to complete to SMC before stopping core
  (void) SMC_PMCTRL;

  SYST_CSR &= ~SYST_CSR_TICKINT; // disable systick timer interrupt
  SCB_SCR |= SCB_SCR_SLEEPDEEP;  // Set SLEEPDEEP bit to enable deep sleep mode (STOP)
  asm volatile( "wfi" );  // WFI instruction will start entry into STOP mode
  // will never return, but generates system reset
}

void setup() {
  pinMode(EXT_LED,     OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // If this is interrpted non stop, no chance for the code to continue,
  // Hence, flashing non stop if I tickle pin 2.
  pinMode(EXT_PIN,     INPUT_PULLUP);
  attachInterrupt(EXT_PIN, ext_pin2_isr, FALLING);

  if ((RCM_SRS0 & RCM_SRS0_WAKEUP) > 0) // low-leakage wakeup reset
    blink(LED_BUILTIN, 500, 3);
  else {
    if ((RCM_SRS0 & RCM_SRS0_POR) > 0)  // power-on reset
      blink(LED_BUILTIN, 500, 6);
    if ((RCM_SRS0 & RCM_SRS0_PIN) > 0)  // external reset pin
      blink(LED_BUILTIN, 500, 5);
    if ((RCM_SRS0 & RCM_SRS0_WDOG)> 0)  // watchdog
      blink(LED_BUILTIN, 500, 4);
    if ((RCM_SRS0 & RCM_SRS0_LOL) > 0)  // loss-of-lock reset
      blink(LED_BUILTIN, 500, 3);
    if ((RCM_SRS0 & RCM_SRS0_LOC) > 0)  // loss-of-clock reset
      blink(LED_BUILTIN, 500, 2);
    if ((RCM_SRS0 & RCM_SRS0_LVD) > 0)  // low-voltage-detect reset
      blink(LED_BUILTIN, 500, 1);

    // Reset not from LLW, clear pending RTC alarm signal
    NVIC_CLEAR_PENDING(IRQ_RTC_ALARM);
  }

  rtc_alarm_setup(5);
  external_pin_setup(); // Enable IRQ_PORTx

  llwu_setup();
  goSleep();
}

// Will never reach here
void loop() {

}