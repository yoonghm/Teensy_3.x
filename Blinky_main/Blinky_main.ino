void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop() methor runs over and over again,
// as long as the board has power

void loop() {
  digitalWrite(LED_BUILTIN, HIGH); delay(1000);
  digitalWrite(LED_BUILTIN, LOW);  delay(1000);
}

int main()
{
  setup();

  while (1)
    loop();
}
