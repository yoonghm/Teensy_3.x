/*
 * This example shows how to interface
 *  - SSD1306 128x64 monochrome OLED
 *  - DS18S20 1-wire digital temperature sensor
 *
 *  Connection
 *
 *  | SSD1306 | Teensy 3.6 | Description                   |
 *  |---------|------------|-------------------------------|
 *  |  DC     |     9      | Data/Command form/to OLED     |
 *  |  RES    |    14      | Reset OLED                    |
 *  |  D1     |    11      | SPI's MOSI pin                |
 *  |  D2     |    13      | SPI's SCK                     |
 *  |  VCC    |    Vin     | Power supply (5 V)            |
 *  |  GND    |    GND     | Ground                        |
 *
 *  | DS18S20 | Teensy 3.6 | Description                   |
 *  |---------|------------|-------------------------------|
 *  |  GND    |    GND     | Ground                        |
 *  |  DQ     |    10      | Data/Command from/to DS18S20  |
 *  |  VDD    |    VIN     | Power supply (5 V)            |
 *
 *  Note:
 *  a. Uncomment "#define SSD1306_128_64" and comment SSD1306_128_32
 *     from Adafruit_SSD1306.h
 *  b. Need a 4.7k ohm resistor between VDD and DQ
 */

#include <Adafruit_SSD1306.h>
#include <TimeLib.h>
#include <OneWire.h>

#if !defined SSD1306_128_64
  #error "You have to define SSD1306_128_64 in Adafruit_SSD1306.h"
#endif

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

  for (int i = 0; i < 9; i++) {
    data[i] = ds.read();
  }

  raw = (data[1] << 8) | data[0];

  if (addr[0] == 0x10) { // DS18S20
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) { // 12-bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  }
  else if ((addr[0] == 0x28) || // DS18B20
           (addr[0] == 0x22)) { // DS1822
    byte cfg = (data[4] & 0x60);
    if (cfg == 0x00) raw = raw & ~7; // 9-bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10-bit resolution, 187.75 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11-bit resolution, 375 ms
    // default is 12-bit resolution, 750 ms conversion time
  }
  snprintf(str, 12, "%0.1fC", (float)(raw) / 16.0);
}

void setup() {
  setSyncProvider(getTeensy3Time);   // Tell TimeLib to use internal RTC

  display.begin(SSD1306_SWITCHCAPVCC); // Generate high voltage internally
  display.setTextColor(WHITE); // White text

  if (ds.search(addr)) {
    if (OneWire::crc8(addr, 7) == addr[7]) { // Address is valid
      present = true;
    }
  }
}

void loop() {

  float    angle; // in radians
  int16_t  x1, x2, y1, y2;

  display.clearDisplay(); // clear internal display buffer

  // Display temperature
  if (present) {
    display.setCursor(65, 2);
    display.setTextSize(2);
    getTemperature();
    display.print(str);
  }

  // Draw the clock face: 1 big and 1 small circle centered at (31,32)
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

  display.setTextSize(1);

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
