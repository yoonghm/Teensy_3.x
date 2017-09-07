#include <Adafruit_SSD1306.h>

#define OLED_DC     9
#define OLED_CS     2
#define OLED_RESET 14

Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);

void setup() {
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

  display.display();
  delay(1000);
}
