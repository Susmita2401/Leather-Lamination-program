#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft; // hard-wired for UNO shields anyway.
#include <TouchScreen.h>

const int XP = 8, XM = A2, YP = A3, YM = 9; // Changed
const int TS_LEFT = 136, TS_RT = 907, TS_TOP = 139, TS_BOT = 942; // Changed

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
TSPoint tp;

#define MINPRESSURE 200
#define MAXPRESSURE 1000

// Assign human-readable names to some common 16-bit color values:
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

void displayVariables(const char *heading, const char *var1, int val1, const char *var2, int val2, int yPos) {
  tft.setTextSize(3); //Heading font size
  
  tft.setTextColor(WHITE);
  tft.setCursor(10, yPos);
  tft.print(heading);
  

  tft.setTextSize(2);

  tft.setCursor(10, yPos +30);       // Adjusted positions for var1
  tft.setTextColor(YELLOW);
  tft.print(var1);

  tft.setCursor(80, yPos + 30);       // Adjusted positions for val1
  tft.setTextColor(CYAN);
  tft.print(val1);

  tft.setCursor(150, yPos +30);      // Adjusted positions for var2
  tft.setTextColor(YELLOW);
  tft.print(var2);

  tft.setCursor(220, yPos +30);      // Adjusted positions for val2
  tft.setTextColor(CYAN);
  tft.print(val2);

}

void setup() {
  tft.begin();
  tft.fillScreen(BLACK);
  Serial.begin(9600);
   tft.setRotation(1);  // Rotate 90 degrees clockwise
}

void loop() {

  // Call displayVariables function to update the display
  displayVariables("Upper Jaw", "Ts1", 0,"Tp1", 0, 30);
  displayVariables("Lower Jaw", "Ts2", 0, "Tp2", 0, 100);
  displayVariables("Time", "S", 0, "P", 0, 170);

  // Draw touch buttons
  drawButton("Up", 290, 40, 80, 30, YELLOW);
  drawButton("+1", 290, 80, 80, 30, GREEN);
  drawButton("-1", 290, 120, 80, 30, RED);
  drawButton("Down", 290, 160, 80, 30, BLUE);

  delay(1000); // Adjust the delay based on your requirements
}

void drawButton(const char *label, int x, int y, int width, int height, uint16_t color) {
  tft.fillRoundRect(x, y, width, height, 5, color);
  tft.setTextSize(2);
  tft.setTextColor(BLACK);
  tft.setCursor(x + 10, y + 10);
  tft.print(label);
}