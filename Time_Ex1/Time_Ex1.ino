#include <TimeLib.h>

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

char datetime[32];

void setup() {
  // Set system time to 23:59:50 31-12-1999
  setTime(23, 59, 50, 31, 12, 1999); 
}

void loop() {
  snprintf(datetime, 32, 
           "%02d:%02d:%02d %02d-%s-%d %s",
           hour(), minute(), second(),
           day(), MonthName[month()],
           year(), WeekdayName[weekday()]
          );
  Serial.println(datetime);
  delay(1000);
}
