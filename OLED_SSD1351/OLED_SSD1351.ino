#include <TimeLib.h>
#include <ssd1351.h>

typedef ssd1351::HighColor Color;
const int size_x = 128;
const int size_y = 128;
// auto display = ssd1351::SSD1351<Color, ssd1351::NoBuffer, size_x, size_y>();
auto display = ssd1351::SSD1351<Color, ssd1351::SingleBuffer, size_x, size_y>();

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

time_t getTeensy3Clock() {
  return Teensy3Clock.get();
}

void displayDigitalClock()
{
  char str[12];
  uint16_t w;

  display.setFont(FreeSans9pt7b);
  display.setTextSize(1);
  display.setTextColor(ssd1351::RGB(255, 255, 255));
  snprintf(str, 12, "%02d-%s-%d", day(), MonthName[month()], year());
  w = display.getTextWidth(str);
  display.setCursor((size_x - w)/2, 20);
  display.print(str);

  display.setFont(FreeSansBold12pt7b);
  display.setTextSize(1);
  display.setTextColor(ssd1351::RGB(0, 64, 255));
  snprintf(str, 9, "%02d:%02d:%02d", hour(), minute(), second());
  display.setCursor(20, 45);
  display.print(str);

  display.setFont(FreeSans12pt7b);
  display.setTextSize(1);
  display.setTextColor(ssd1351::RGB(255, 255, 255));
  snprintf(str, 9, "%s", WeekdayName[weekday()]);
  w = display.getTextWidth(str);
  display.setCursor((size_x - w)/2, 70);
  display.print(str);
}

void setup() {
  Serial.println("Hi");

  setSyncProvider(getTeensy3Clock);

  display.begin();
  display.fillScreen(ssd1351::RGB());

#if 0
  display.drawPixel(0, 0, ssd1351::RGB(255,255,255));
  display.drawPixel(size_x/2, size_y/2, ssd1351::RGB(255,255,255));
  display.drawPixel(size_x-1, size_y-1, ssd1351::RGB(255,255,255));
#endif

  display.updateScreen();
}

void loop() {
  display.fillScreen(ssd1351::RGB());
  displayDigitalClock();
  display.updateScreen();
}
