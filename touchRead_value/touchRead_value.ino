/*
 * Open Serial Monitor window to monitor serial port
 * Capacitance value (pF) is very stable
 */
void setup()   {                
  Serial.begin(38400);
}

void loop()                     
{
  int cap = touchRead(16);
  Serial.println(cap);
  delay(200);
}
