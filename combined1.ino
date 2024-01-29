#include <SPI.h>  //Thermocouple library
#include "max6675.h"


#include <TFT_HX8357.h>  //Display Library
#include "Free_Fonts.h"

// Define the connections to the first MAX6675 Thermo
int thermo1SO = 4;
int thermo1CS = 3;
int thermo1SCK = 2;

// Define the connections to the second MAX6675 Thermo
int thermo2SO = 7;
int thermo2CS = 6;
int thermo2SCK = 5;

// Define the pins for the LEDs
int ledPin1 = 10;
int ledPin2 = 12;

// Define the pins for the relays
int relay1Pin = 22; // Change this to the appropriate relay pin connected to control the relay for LED 1
int relay2Pin = 24; // Change this to the appropriate relay pin connected to control the relay for LED 2

// Define the threshold temperatures (in Celsius) to trigger the LEDs for each thermocouple
const int thresholdTemp1 = 28; // Change this value to your desired threshold temperature for thermocouple 1
const int thresholdTemp2 = 27; // Change this value to your desired threshold temperature for thermocouple 2

MAX6675 thermocouple1(thermo1SCK, thermo1CS, thermo1SO);
MAX6675 thermocouple2(thermo2SCK, thermo2CS, thermo2SO);

TFT_HX8357 tft = TFT_HX8357();
const char *scrollingText = "This is a DEMO Project";

void setup() {
  Serial.begin(9600);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);

  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);  //Before uploading make screen black

  tft.setTextSize(1);  //Font Size
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);

  // Display headings and placeholders
  displayVariables("Upper Jaw Temperature", "Tp1", 0, "Ts1", thresholdTemp1, 50);    //Initial Y-Pos
  displayVariables("Lower Jaw Temperature", "Tp2", 0, "Ts2", thresholdTemp2, 140);   //2nd Y-Pos
  displayVariables("Time", "P", 0, "S", 0, 230);   //3rd Y-Pos

  // Hold the initial values for a short duration
  delay(2000);
}

void loop() {
  // Read the temperature from the first thermocouple
  double celsius1 = thermocouple1.readCelsius();

  // Read the temperature from the second thermocouple
  double celsius2 = thermocouple2.readCelsius();

  // Update display with temperature values
  updateDisplay(celsius1, celsius2);

  // Check and update LED states based on temperature thresholds
  updateLEDs(celsius1, celsius2);

  delay(1000); // Adjust delay time as needed for reading frequency
}

// Project Title
void scrollText() {
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextSize(2);

  const char *staticText = "I E R C";
  tft.setCursor(20, 10); // (X,Y)
  tft.print(staticText);
}

void displayVariables(const char *heading, const char *varName1, double varValue1, const char *varName2, double varValue2, int yPos) {
  tft.setFreeFont(FSB9);
  tft.setCursor(10, yPos);
  tft.print(heading);
  tft.setCursor(10, yPos + 40);
  tft.print(varName1);
  tft.setCursor(80, yPos + 40);
  tft.print(varValue1, 1); // Print actual temperature with one decimal place
  
  tft.setCursor(160, yPos + 40);
  tft.print(varName2);
  tft.setCursor(230, yPos + 40);
  tft.print(varValue2, 1); // Print set temperature with one decimal place

 
}



// Update displayed values with new temperatures
void updateDisplay(double temp1, double temp2) {
  displayVariables("Upper Jaw Temperature", "Tp1", temp1, "Ts1", thresholdTemp1, 50);
  displayVariables("Lower Jaw Temperature", "Tp2", temp2, "Ts2", thresholdTemp2, 140);
  displayVariables("Time", "P", 0, "S", 0, 230); // Update time if needed
}


//LED functionality

void updateLEDs(double temp1, double temp2) {
  // Check and update LED states based on temperature thresholds
  if (temp1 >= thresholdTemp1) {
    digitalWrite(ledPin1, LOW); // Turn on LED 1

    //digitalWrite(relay1Pin, LOW); // Turn on Relay 1
   // Serial.println("Relay 1 is OFF");
  } else {
    digitalWrite(ledPin1, HIGH); // Turn off LED 1


    //digitalWrite(relay1Pin, HIGH); // Turn off Relay 1
    //Serial.println("Relay 1 is ON");
  }

  if (temp2 >= thresholdTemp2) {
    digitalWrite(ledPin2, LOW); // Turn on LED 2


    //digitalWrite(relay2Pin, LOW); // Turn on Relay 2
    //Serial.println("Relay 2 is OFF");
  } else {
    digitalWrite(ledPin2, HIGH); // Turn off LED 2


    //digitalWrite(relay2Pin, HIGH); // Turn off Relay 2
    //Serial.println("Relay 2 is ON");
  }
}
