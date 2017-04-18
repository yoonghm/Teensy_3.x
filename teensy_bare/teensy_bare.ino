void setup() {
  SIM_SCGC5 |= SIM_SCGC5_PORTC; // Enable clock to PORTC
  PORTC_PCR5 |= PORT_PCR_MUX(1) | PORT_PCR_DSE; // GPIO, High Drive Strength
  GPIOC_PDDR |= (1<<5);  // Configure PORTC Pin #5 as output 

}

void wait(uint32_t del)
{
  volatile uint32_t index;

  for (index = 0; index < del; index++);
}

void loop() {

  GPIOC_PSOR |= (1<<5);  // Set PORTC Pin #5 to high
  wait(8000000);
  GPIOC_PCOR |= (1<<5);  // Set PORTC Pin #5 to low
  wait(8000000);
}