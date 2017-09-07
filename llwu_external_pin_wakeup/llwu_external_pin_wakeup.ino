#define EXT_LED  0
#define EXT_PIN  2   // PTD0 - LLWU_P12

void board_setup()
{
//SIM_SCGC5 |= SIM_SCGC5_PORTD;  // Enable clock gating for PORTD
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(EXT_LED, OUTPUT);
  pinMode(EXT_PIN, INPUT_PULLUP);
  NVIC_CLEAR_PENDING(IRQ_PORTD);
  NVIC_IS_ENABLED(IRQ_PORTD); // pin 2 is in PORTD
  attachInterrupt(EXT_PIN, external_pin_isr, FALLING);  
}

void blink(uint8_t pin, uint32_t delay_ms, uint32_t count) {
  for (; count; --count) {
    digitalWriteFast(pin, !digitalReadFast(pin));
    delay(delay_ms);
    digitalWriteFast(pin, !digitalReadFast(pin));
    delay(delay_ms);
  }
}

void external_pin_isr() {
  // Don't bother the exact wakeup pin, just clear all the flags
  LLWU_PE1 = 0; LLWU_PE2 = 0; LLWU_PE3 = 0; LLWU_PE4 = 0;
#if defined(HAS_KINETIS_LLWU_32CH)
  LLWU_PE5 = 0; LLWU_PE6 = 0; LLWU_PE7 = 0; LLWU_PE8 = 0;
#endif

  // No need to clear PORTx_ISFR as NVIC was done during VLLSx

  // Restore chip configuration before clearning PMC_REGSC_ACKISO
  board_setup();
  PMC_REGSC |= PMC_REGSC_ACKISO;

  blink(EXT_LED, 200, 1);
}

int llwu_pinEnable(uint32_t pin, uint32_t mode) {
  int done = 0;

  if (mode < LLWU_PE_WUPE_PIN_DISABLE ||
      mode > LLWU_PE_WUPE_PIN_ANY)
    return -1;

  switch (pin) {
    case  4: LLWU_PE2 |= LLWU_PE2_WUPE4 (mode);
    case  9: LLWU_PE2 |= LLWU_PE2_WUPE7 (mode);
    case 16: LLWU_PE2 |= LLWU_PE2_WUPE5 (mode);
    case 22: LLWU_PE2 |= LLWU_PE2_WUPE6 (mode);

    case 10: LLWU_PE3 |= LLWU_PE3_WUPE8 (mode);
    case 11: LLWU_PE3 |= LLWU_PE3_WUPE10(mode);
    case 13: LLWU_PE3 |= LLWU_PE3_WUPE9 (mode);

    case  2: LLWU_PE4 |= LLWU_PE4_WUPE12(mode);
    case  7: LLWU_PE4 |= LLWU_PE4_WUPE13(mode);
    case  6: LLWU_PE4 |= LLWU_PE4_WUPE14(mode);
    case 21: LLWU_PE4 |= LLWU_PE4_WUPE15(mode);

#if defined(HAS_KINETIS_LLWU_32CH)
    case 34: LLWU_PE6 |= LLWU_PE6_WUPE21(mode);
    case 47: LLWU_PE7 |= LLWU_PE7_WUPE24(mode);
    case 55: LLWU_PE7 |= LLWU_PE7_WUPE25(mode);
    case 56: LLWU_PE5 |= LLWU_PE5_WUPE18(mode);
#endif
    done = 1;
  }

  if (done) {
    NVIC_ENABLE_IRQ(IRQ_LLWU);
    return 0;
  }
  else
    return -2;
}

/*
 * mode:
 *   0 - Normal run mode (RUN)
 *   1 - Reserved
 *   2 - Very-low-power run mode (VLPR)
 *   3 - High-speed run mode
 */
void high_speed_run(uint8_t mode) {
  volatile uint32_t dummyread;

  SMC_PMPROT = SMC_PMPROT_AHSRUN;
  
  /* Can go into RUN mode only when it was in the HSRUN or VLPR mode */
  if ( (mode == 0) && 
       ( (SMC_PMSTAT & SMC_PMSTAT_VLPR) || 
         (SMC_PMSTAT & SMC_PMSTAT_HSRUN)
       )
     )
    SMC_PMCTRL |= SMC_PMCTRL_RUNM(0);

  /* Can go into VLRP mode only when it was in the RUN mode */
  else if ( (mode == 2) &&
            (SMC_PMSTAT & SMC_PMSTAT_RUN)
          )
    SMC_PMCTRL |= SMC_PMCTRL_RUNM(2);

  /* Can go into HSRUN mode only when it was in the RUN mode */
  else if ( (mode == 3) &&
            (SMC_PMSTAT & SMC_PMSTAT_RUN)
          )
    SMC_PMCTRL |= SMC_PMCTRL_RUNM(3);
}

void very_low_power_sleep(uint8_t mode) {
  volatile uint32_t dummyread;

  SMC_PMPROT = SMC_PMPROT_AVLP;

}

void low_leakage_sleep(uint8_t mode) {
  volatile uint32_t dummyread;

  SMC_PMPROT = SMC_PMPROT_ALLS;
}

void very_low_leakage_sleep(uint8_t mode) {
  volatile uint32_t dummyread;

  SMC_PMPROT = SMC_PMPROT_AVLLS;
}

void setup() {
  board_setup();
  llwu_pinEnable(2, LLWU_PE_WUPE_PIN_ANY);
  sleep(xxx);
}

// Never reach here
void loop() {
  blink(LED_BUILTIN, 200, 1);
}