#include <TimeLib.h>

const int DATETIMELEN = 32;
char DateTime[DATETIMELEN];

int status = 0;

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

time_t getTeensy3Clock() {
  return Teensy3Clock.get();
}

void setup()  {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  // Use Teensy3Clock - a global teensy3_clock_class object
  setSyncProvider(getTeensy3Clock);

  if (timeStatus() != timeSet) {
    Serial.println("Time's clock is not set or not synchroized");
  }
}

void loop() {
  status = !status;
  digitalWrite(LED_BUILTIN, status);

  snprintf(DateTime, DATETIMELEN,
           "%02d-%s-%d %s %02d:%02d:%02d",
           day(), MonthName[month()], year(), WeekdayName[weekday()],
           hour(), minute(), second()
          );
  Serial.println(DateTime);
  delay(1000);
}
