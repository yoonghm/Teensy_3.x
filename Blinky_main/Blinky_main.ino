/*
 * This example illustrates that we can override the default main() function.
 */

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

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
