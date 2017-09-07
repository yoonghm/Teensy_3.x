#include <Adafruit_SSD1306.h>
#include <TimeLib.h>
#include <OneWire.h>

#define OLED_DC     9
#define OLED_CS     2
#define OLED_RESET 14

Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);

// weekday() returns integer: 1 = Sunday
const char *WeekdayName[] = {
  "      "   , // 0
  "Sunday"   , // 1
  "Monday"   , // 2
  "Tuesday"  , // 3
  "Wednesday", // 4
  "Thursday" , // 5
  "Friday"   , // 6
  "Saturday"   // 7
};

// month() returns integer: 1 = January
const char *MonthName[] = {
  "   ",       // 0
  "Jan",       // 1
  "Feb",       // 2
  "Mar",       // 3
  "Apr",       // 4
  "May",       // 5
  "Jun",       // 6
  "Jul",       // 7
  "Aug",       // 8
  "Sep",       // 9
  "Oct",       // 10
  "Nov",       // 11
  "Dec"        // 12
};

char       str[12];
time_t     t;

OneWire    ds(10);  // Need a 4.7k resistor between pin 10 and VCC
bool       present;
byte       addr[8]; // 8-byte unique ID for DS18x20
byte       type_s;

// Function to retrieve time in seconds in 1-1-1972, from internal RTC
time_t getTeensy3Time() {
  return Teensy3Clock.get();
}

void getTemperature() {
  byte     data[12];
  int16_t  raw;

  ds.reset();
  ds.select(addr);
  ds.write(0xBE); // Read scratchpad

  for (int i = 0; i < 9; i++) { // 9 bytes
    data[i] = ds.read();
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.print(OneWire::crc8(data, 8), HEX);
  Serial.println();
  
  raw = (data[1] << 8) | data[0];
  Serial.println((float)raw);
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) { // 12-bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
    Serial.print("type_s = ");
    Serial.println(raw);
  }
  else {
    byte cfg = (data[4] & 0x60);
    if (cfg == 0x00) raw = raw & ~7; // 9-bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10-bit resolution, 187.75 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11-bit resolution, 375 ms
    // default is 12-bit resolution, 750 ms conversion time
  }
  snprintf(str, 12, "%0.1fC", (float)(raw) /16.0);
}

void setup() {
  // Setup TimeLib with RTC
  setSyncProvider(getTeensy3Time);   // Tell TimeLib to use internal RTC

  // Setup SSD1306 OLED
  display.begin(SSD1306_SWITCHCAPVCC); // Generate high voltage internally
  display.setTextColor(WHITE); // White text

  // Setup temperature sensor
  if (ds.search(addr)) {
    Serial.print("ROM =");
    for (int i = 0; i < 8; i++) {
      Serial.write(' ');
      Serial.print(addr[i], HEX);
    }
    Serial.println();
    if (OneWire::crc8(addr, 7) == addr[7]) {
      Serial.println("Addr's CRC is valid");
      present = true;
    }
  }

  if (addr[0] == 0x10)      type_s = 1; // DS18S20, i.e., old DS1820
  else if (addr[0] == 0x28) type_s = 0; // DS18B20
  else if (addr[0] == 0x22) type_s = 0; // DS1822
  Serial.print("type_s = ");
  Serial.println(type_s);
}

void loop() {

  float    angle; // in radians
  int16_t  x1, x2, y1, y2;

  display.clearDisplay(); // clear internal display buffer

  // Display temperature
  display.setCursor(65, 2);
  display.setTextSize(2);
  getTemperature();
  display.print(str);
  
  display.drawCircle(31, 32, 30, WHITE);
  display.drawCircle(31, 32,  2, WHITE);

  // Draw ticks:
  // A small line at every 30 deg clockwise from vertical axis
  for (int z = 0; z < 360; z += 30) { // Clockwise angle from vertical axis
    angle = z / 57.29577951; // Convert degrees to radians;
    x1 = 31 + 30*sin(angle);
    y1 = 32 - 30*cos(angle);
    x2 = 31 + 28*sin(angle);
    y2 = 32 - 28*cos(angle);
    display.drawLine(x1, y1, x2, y2, WHITE);
  }

  display.setTextSize(1);

  t = now();
  
  // Draw second hand:
  // Every second occupies 6 deg
  angle = second(t)*6 / 57.29577951;
  x1 = 31 + 28*sin(angle);
  y1 = 32 - 28*cos(angle);
  display.drawLine(31, 32, x1, y1, WHITE);

  // Draw minute hand:
  // Every minute occupies 6 deg
  angle = minute(t)*6 / 57.29577951;
  x1 = 31 + 20*sin(angle);
  y1 = 32 - 20*cos(angle);
  display.drawLine(31, 32, x1, y1, WHITE);

  // Draw hour hand:
  // In 12 hours (or 720 min), hour hand moves 360 deg
  // I.e., in every min, it move 2 deg
  angle = (hourFormat12(t)*60 + minute(t)) / 2 / 57.29577951;
  x1 = 31 + 15*sin(angle);
  y1 = 32 - 15*cos(angle);
  display.drawLine(31, 32, x1, y1, WHITE);

  // Print hh:mm:ss
  snprintf(str, 12, "%02d:%02d:%02d", hour(), minute(), second());
  display.setCursor(68, 35);
  display.print(str);

  // Print weekday
  snprintf(str, 12, "%s", WeekdayName[weekday()]);
  display.setCursor(68, 45);
  display.print(str);

  // Print dd-mm-yyyy
  snprintf(str, 12, "%02d-%s-%d", day(), MonthName[month()], year());
  display.setCursor(61,55);
  display.print(str);

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1); // Start conversion with parasite power on at the end
  // Need to wait > 750 ms to read temperature, do it in next loop

  display.display();
  delay(1000);
}
