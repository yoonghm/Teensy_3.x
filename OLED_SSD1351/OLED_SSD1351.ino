#include <Arduino.h>
#include <SPI.h>
#include <ssd1351.h>
#include <TimeLib.h>

typedef ssd1351::HighColor Color;
const int size_x = 128;
const int size_y = 128;
// auto display = ssd1351::SSD1351<Color, ssd1351::NoBuffer, size_x, size_y>();
auto display = ssd1351::SSD1351<Color, ssd1351::SingleBuffer, size_x, size_y>();

volatile int timer_1s = 0;
volatile int status = 0;

// weekday() returns integer: 1 = Sunday
const char *WeekdayName[] = {
  "      "   ,   // 0
  "Sunday"   ,   // 1
  "Monday"   ,   // 2
  "Tuesday"  ,   // 3
  "Wednesday",   // 4
  "Thursday" ,   // 5
  "Friday"   ,   // 6
  "Saturday"     // 7
};

// month() return integer: 1 = January
const char *MonthName[] = {
  "   "     ,    // 0
  "Jan"     ,    // 1
  "Feb"     ,    // 2
  "Mar"     ,    // 3
  "Apr"     ,    // 4
  "May"     ,    // 5
  "Jun"     ,    // 6
  "Jul"     ,    // 7
  "Aug"     ,    // 8
  "Sep"     ,    // 9
  "Oct"     ,    // 10
  "Nov"     ,    // 11
  "Dec"          // 12
};

void systick_isr() {
  systick_millis_count++;   // Original ISR code
  if (systick_millis_count % 1000 == 0) {
    timer_1s = 1;
  }
}

time_t getTeensy3Clock() {
  return Teensy3Clock.get();
}

void printDate()
{
  display.setFont(FreeSans9pt7b);
  display.setTextSize(1);
  display.setTextColor(ssd1351::RGB(255, 255, 255));

  char str[12];
  snprintf(str, 12, "%02d-%s-%d", day(), MonthName[month()], year());
#if 0
  snprintf(datetime, DATETIMELEN,
           "%02d-%s-%d %s %02d:%02d:%02d",
           day(), MonthName[month()], year(), WeekdayName[weekday()],
           hour(), minute(), second()
          );
#endif
  uint16_t w = display.getTextWidth(str);
  display.setCursor((size_x - w)/2, 20);
  display.print(str);
}

void setup() {
  Serial.println("Hi");

  display.begin();
  display.fillScreen(ssd1351::RGB());

  setSyncProvider(getTeensy3Clock);
  printDate();

//display.drawPixel(0, 0, ssd1351::RGB(255,255,255));
//display.drawPixel(size_x/2, size_y/2, ssd1351::RGB(255,255,255));
//display.drawPixel(size_x-1, size_y-1, ssd1351::RGB(255,255,255));

  display.updateScreen();
  display.end();
}

void loop() {

  if (timer_1s == 1)
  {
    timer_1s = 0;
    display.begin();
    printDate();
    display.updateScreen();
    display.end();
    status = !status;
  }
}
