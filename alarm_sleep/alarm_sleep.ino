void flashLed(int del)
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(del);
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(LED_BUILTIN, INPUT); // Conserve leakage current
}

/******************* Seting Alarm **************************/
#define RTC_IER_TAIE_MASK       0x4u
#define RTC_SR_TAF_MASK         0x4u

// Enable RTC module and 32.768 kHz oscillator
void rtcSetup(void) {

  // Enable software access and interrupts to RTC module
  SIM_SCGC6 |= SIM_SCGC6_RTC;

  // Enable 32.768 kHz oscillator
  RTC_CR |= RTC_CR_OSCE;
}

void rtcSetAlarm(uint32_t nsec) {
  RTC_TAR = RTC_TSR + nsec;
  RTC_IER |= RTC_IER_TAIE_MASK;
}

/********************LLWU**********************************/
#define LLWU_ME_WUME5_MASK       0x20u
#if defined (HAS_KINETIS_LLWU_32CH)
#define LLWU_MF5_WMUF5_MASK      0x20u
#elif defined (HAS_KINETIS_LLWU_16CH)
#define LLWU_F3_MWUF5_MASK       0x20u
#endif

static void llwuISR(void)
{
#if defined (HAS_KINETIS_LLWU_32CH)
  LLWU_MF5 |= LLWU_MF5_WMUF5_MASK;
#elif defined (HAS_KINETIS_LLWU_16CH)
  LLWU_F3 |= LLWU_F3_MWUF5_MASK; // clear source in LLWU Flag register
#endif
  RTC_IER = 0;// clear RTC interrupts
}

void llwuSetup(void)
{
  attachInterruptVector(IRQ_LLWU, llwuISR);
  NVIC_SET_PRIORITY(IRQ_LLWU, 2*16); // 3rd highest priority

  NVIC_CLEAR_PENDING(IRQ_LLWU);
  NVIC_ENABLE_IRQ(IRQ_LLWU);

  LLWU_PE1 = 0; // Disable all external wakeup input pins  0..3
  LLWU_PE2 = 0; // Disable all external wakeup input pins  4..7
  LLWU_PE3 = 0; // Disable all external wakeup input pins  8..11
  LLWU_PE4 = 0; // Disable all external wakeup input pins 12..15
  LLWU_ME  = LLWU_ME_WUME5_MASK; //rtc alarm
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
   rtcSetup();
   llwuSetup();

   rtcSetAlarm(5);
   goSleep();
}

void loop() {
   // put your main code here, to run repeatedly:
   //
}