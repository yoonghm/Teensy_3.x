#include <Adafruit_SSD1306.h>
#include <TimeLib.h>

#define OLED_DC     9
#define OLED_CS     2
#define OLED_RESET 14

Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);
time_t           t;

// Function to retrieve time in seconds in 1-1-1972, from internal RTC
time_t getTeensy3Time() {
  return Teensy3Clock.get();
}

void setup() {
  setSyncProvider(getTeensy3Time);   // Tell TimeLib to use internal RTC

  display.begin(SSD1306_SWITCHCAPVCC); // Generate high voltage internally
  display.setTextColor(WHITE); // White text
}

void loop() {

float    angle; // in radians
int16_t  x1, x2, x3, y1, y2, y3;

  display.clearDisplay(); // clear internal display buffer
  display.drawCircle(64, 32, 30, WHITE);
  display.drawCircle(64, 32,  2, WHITE);

  // Draw ticks:
  // A small line at every 30 deg clockwise from vertical axis
  for (int z = 0; z < 360; z += 30) { // Clockwise angle from vertical axis
    angle = z / 57.29577951; // Convert degrees to radians;
    x1 = 64 + 30*sin(angle);
    y1 = 32 - 30*cos(angle);
    x2 = 64 + 28*sin(angle);
    y2 = 32 - 28*cos(angle);
    display.drawLine(x1, y1, x2, y2, WHITE);
  }
  t = now();
  
  // Draw second hand:
  // Every second occupies 6 deg
  angle = second(t)*6 / 57.29577951;
  x3 = 64 + 28*sin(angle);
  y3 = 32 - 28*cos(angle);
  display.drawLine(64, 32, x3, y3, WHITE);
  
  display.display();
  delay(1000);
}
