void flashLed(int del)
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(del);
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(LED_BUILTIN, INPUT);
}

/******************* Seting Alarm **************************/
#define RTC_IER_TAIE_MASK       0x4u
#define RTC_SR_TAF_MASK         0x4u

void rtcSetup(uint32_t nsec)
{
  SIM_SCGC6 |= SIM_SCGC6_RTC;// enable RTC clock
  RTC_CR |= RTC_CR_OSCE;// enable RTC

  RTC_TAR = RTC_TSR + nsec;
  RTC_IER |= RTC_IER_TAIE_MASK;
}

/********************LLWU**********************************/
#define WMUF5_MASK      0x20u

static void llwuISR(void)
{
#if defined(HAS_KINETIS_LLWU_32CH)
  LLWU_MF5 |= WMUF5_MASK;
#elif defined(HAS_KINETIS_LLWU_16CH)
  LLWU_F3 |= WMUF5_MASK; // clear source in LLWU Flag register
#endif
  RTC_IER = 0;// clear RTC interrupts
}

void llwuSetup(void)
{
  attachInterruptVector( IRQ_LLWU, llwuISR );
  NVIC_SET_PRIORITY( IRQ_LLWU, 2*16 );

  NVIC_CLEAR_PENDING( IRQ_LLWU );
  NVIC_ENABLE_IRQ( IRQ_LLWU );

  LLWU_PE1 = 0;
  LLWU_PE2 = 0;
  LLWU_PE3 = 0;
  LLWU_PE4 = 0;
  LLWU_ME  = LLWU_ME_WUME5; //rtc alarm
}

/********************* go to deep sleep *********************/
// These are defined in the core now
#define SMC_PMPROT_AVLLS_MASK   0x2u
#define SMC_PMCTRL_STOPM_MASK   0x7u
#define SCB_SCR_SLEEPDEEP_MASK  0x4u

void goSleep(void) {
  //volatile unsigned int dummyread;
  /* Make sure clock monitor is off so we don't get spurious reset */
  // currently not set by anything I know, so the clock monitor is not set from reset
  MCG_C6 &= ~MCG_C6_CME0;
  //
  /* Write to PMPROT to allow all possible power modes */
  SMC_PMPROT = SMC_PMPROT_AVLLS_MASK;// Not needed already taken care of in here.
  /* Set the STOPM field to 0b100 for VLLSx mode */
  SMC_PMCTRL &= ~SMC_PMCTRL_STOPM_MASK;
  SMC_PMCTRL |= SMC_PMCTRL_STOPM(0x4); // VLLSx

  SMC_VLLSCTRL =  SMC_VLLSCTRL_VLLSM(0x3); // VLLS3
  /*wait for write to complete to SMC before stopping core */
  (void) SMC_PMCTRL;

  SYST_CSR &= ~SYST_CSR_TICKINT;      // disable systick timer interrupt
  SCB_SCR |= SCB_SCR_SLEEPDEEP_MASK;  // Set SLEEPDEEP bit to enable deep sleep mode (STOP)
  asm volatile( "wfi" );  // WFI instruction will start entry into STOP mode
  // will never return, but generates system reset
}

void setup() {
   //
   // put your setup code here, to run once:
   flashLed(100);
   //
   rtcSetup(5);
   llwuSetup();

   goSleep();
}

// Never reach here
void loop() {
}