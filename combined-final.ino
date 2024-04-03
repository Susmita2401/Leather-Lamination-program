#include <SPI.h>
#include "max6675.h"
#include <UTFT.h>
#include <ClickEncoder.h>
#include <TimerOne.h>

const int encoderPinA = 2;  // CLK or A phase pin
const int encoderPinB = 3;  // DT or B phase pin
const int encoderSwitchPin = 4;  // SW or switch pin
//2 pins to ground required
// Declare which fonts we will be using
extern uint8_t GroteskBold32x64[];
extern uint8_t GroteskBold16x32[];
UTFT myGLCD(CTE40,38,39,40,41);

String menuItem1 = "> RUN ";
String menuItem2 = "> Set Temp(";
String menuItem3 = "  Present Temp(";
String menuItem4 = "> Set Time (s)";
String menuItem5 = "  Present Time (s)";
String menuItem6 = "> Reset";

static int menuitem = 1;
static int page=1;
static boolean status = false;
static double stemp=100.00;
double ptemp;
static int stime = 30;
int ptime;

boolean up = false;
boolean down = false;
boolean middle = false;

ClickEncoder *encoder;
int16_t last, value;

// Thermocouple Pins
int thermo1SO = 5;
int thermo1CS = 6;
int thermo1SCK = 7;

// Thermocouple instances
MAX6675 thermocouple1(thermo1SCK, thermo1CS, thermo1SO);

// Function prototypes for Relays
void turnOffAllRelays();
void manageRelay3State();
void manageRelay4State();

bool thermocoupleValuesAvailable = false; // Flag to track if all thermocouple values are available

bool r = false;

// Relay Pins
int relay1Pin = 11;
int relay2Pin = 10;
int relay3Pin = 9;
int relay4Pin = 8;

// Time variables for Relay 3
unsigned long tOn = 5;  // First tOn should be high
bool firstLoop = true; // Flag to track if it's the first loop iteration

unsigned long tOff = 10;
unsigned long counter = 0;

// Variables for Set condition when Ts1 = Tp1 
int m = 0;

void setup() {
  Serial.begin(9600);
  //Set up LCD
  myGLCD.InitLCD();
  myGLCD.setFont(GroteskBold16x32);
  myGLCD.clrScr();
  myGLCD.fillScr(VGA_WHITE);
  // EC11 Rotary Encoder pins
  pinMode(encoderPinA,INPUT);
  pinMode(encoderPinB,INPUT);
  pinMode(encoderSwitchPin,INPUT);
    
  encoder = new ClickEncoder(encoderPinA, encoderPinB, encoderSwitchPin);
  encoder->setAccelerationEnabled(false);
   
  Timer1.initialize(100);
  Timer1.attachInterrupt(timerIsr); 
  last = encoder->getValue();
  // Relay Pin Mode & State
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);
  pinMode(relay3Pin, OUTPUT);

  // Relays are in off state initially
  digitalWrite(relay1Pin, LOW); //Relay 1 and 2 is ON when Power is ON
  digitalWrite(relay2Pin, LOW);
  digitalWrite(relay3Pin, HIGH); //Relay4 is off
}

void loop() {
  // Reading for Thermocouples
  ptemp = thermocouple1.readCelsius();
  Serial.println(ptemp);
  
  if (ptemp >= stemp) {
    m = 1;
  }
  else if(ptemp < (stemp-1)){
    digitalWrite(relay2Pin, LOW);
  }
  if (isnan(ptemp)) {
    turnOffAllRelays();
    thermocoupleValuesAvailable = false;
    while(isnan(ptemp)){
      myGLCD.setFont(GroteskBold32x64);
      myGLCD.setColor(VGA_WHITE);
      myGLCD.fillScr(VGA_RED); // Erase the screen
      myGLCD.print("* WARNING *", CENTER, 100);
      myGLCD.print("No Reading ",CENTER, 250);
      myGLCD.print("Available",CENTER, 350);
    }
    myGLCD.fillScr(VGA_WHITE); // Erase the screen
    myGLCD.setFont(GroteskBold16x32);
    myGLCD.setColor(VGA_BLACK);
  } 
  // Update tOn for subsequent loop iterations
  if (firstLoop) {
    tOn = 2; // Change tOn to 2 after the first loop
    firstLoop = false;
  }

  // Print Variables on serial monitor
  Serial.print("Ts1:");
  Serial.print(stemp);
  Serial.print(", Tp1:");
  Serial.print(ptemp);
  Serial.print(", S:");
  Serial.print(stime);
  Serial.print(", P:");
  Serial.print(ptime);
  Serial.print(", State:");
  Serial.println(r);
  // If all thermocouple values are available, update the display
  if (thermocoupleValuesAvailable) {
    menu();
    readRotaryEncoder();
  
    if (up && page==1){
      up=false;
      menuitem++;
      if (menuitem>=7){
        menuitem=1;  
      }
    }
    else if (up && page==2 && menuitem==2){
      up= false;
      stemp++;
      if (stemp>300){
        stemp=300;
      }
    }
    else if (up && page==2 && menuitem==4){
      up= false;
      stime++;
      if (stime>50){
        stime=50;
      }
    }
    if (down && page==1){
      down=false;
      menuitem--;
      if (menuitem<=1){
        menuitem=7;  
      }
    }
    else if (down && page==2 && menuitem==2){
      down= false;
      stemp--;
      if (stemp<0){
        stemp=0;
      }
    }
    else if (down && page==2 && menuitem==4){
      down= false;
      stime--;
      if (stime<0){
        stime=0;
      }
    }
    if (middle && page==1){
      middle=false;
      page =2;
      if (menuitem==3 || menuitem==5){
        page=1;
      }
      else if (menuitem==1){
        page=1;
        if (status){
          manageRelay2State();
          manageRelay3State();
          status=false;
          menuItem1="> RUN ";
          systemRun();
        }
        else{
          status=true;
          menuItem1="> STOP";
          systemStop();
        }
      }
      else if (menuitem==6){
        systemReset();
        Serial.println("System has been RESET");
      }
    }
    else if (middle && page==2){
      middle=false;
      page=1;
    }
  }
}
void menu(){
      myGLCD.setColor(VGA_BLACK);
      myGLCD.setBackColor(VGA_TRANSPARENT);
      myGLCD.print("Leather Lamination Machine", CENTER, 2);
      myGLCD.drawLine(5,40, 475,40);
      myGLCD.drawLine(10,45, 470,45);
      switch(menuitem){
        case 1: 
        displayMenuItem(menuItem1, 70, true);
        displayMenuItem(menuItem2, 110, false);
        displayMenuItem(menuItem3, 150, false);
        displayMenuItem(menuItem4, 190, false);
        displayMenuItem(menuItem5, 230, false);
        displayMenuItem(menuItem6, 270, false);
        break;
        case 2:
        displayMenuItem(menuItem1, 70, false);
        displayMenuItem(menuItem2, 110, true);
        displayMenuItem(menuItem3, 150, false);
        displayMenuItem(menuItem4, 190, false);
        displayMenuItem(menuItem5, 230, false);
        displayMenuItem(menuItem6, 270, false);
        break;
        case 3:
        displayMenuItem(menuItem1, 70, false);
        displayMenuItem(menuItem2, 110, false);
        displayMenuItem(menuItem3, 150, true);
        displayMenuItem(menuItem4, 190, false);
        displayMenuItem(menuItem5, 230, false);
        displayMenuItem(menuItem6, 270, false);
        break;
        case 4:
        displayMenuItem(menuItem1, 70, false);
        displayMenuItem(menuItem2, 110, false);
        displayMenuItem(menuItem3, 150, false);
        displayMenuItem(menuItem4, 190, true);
        displayMenuItem(menuItem5, 230, false);
        displayMenuItem(menuItem6, 270, false);
        break;
        case 5:
        displayMenuItem(menuItem1, 70, false);
        displayMenuItem(menuItem2, 110, false);
        displayMenuItem(menuItem3, 150, false);
        displayMenuItem(menuItem4, 190, false);
        displayMenuItem(menuItem5, 230, true);
        displayMenuItem(menuItem6, 270, false);
        break;
        case 6:
        displayMenuItem(menuItem1, 70, false);
        displayMenuItem(menuItem2, 110, false);
        displayMenuItem(menuItem3, 150, false);
        displayMenuItem(menuItem4, 190, false);
        displayMenuItem(menuItem5, 230, false);
        displayMenuItem(menuItem6, 270, true);
        break;
      }
    }
  void displayMenuItem(String item, int position, boolean selected){
    if(selected)
    {
      myGLCD.setBackColor(VGA_BLACK);
      myGLCD.setColor(VGA_WHITE);
      myGLCD.print(item, 2, position);
      if (item==menuItem2){
        myGLCD.print("o",180, position-8);
        myGLCD.print("C)",195, position);
        myGLCD.printNumF(stemp,2, RIGHT, 110);
      }
      else if(item==menuItem3){
        myGLCD.print("o",240, position-8);
        myGLCD.print("C)",255, position);
        myGLCD.printNumF(ptemp,2, RIGHT, 150);
        }
      else if(item==menuItem4){
        myGLCD.printNumI(stime, RIGHT, 190);
      }
      else if (item==menuItem5){
        myGLCD.printNumI(ptime, RIGHT, 230);
      }
    }else
    {
      myGLCD.setBackColor(VGA_WHITE);
      myGLCD.setColor(VGA_BLACK);
      myGLCD.print(item, 2, position);
      if (item==menuItem2){
        myGLCD.print("o",180, position-8);
        myGLCD.print("C)",195, position);
        myGLCD.setColor(VGA_RED);
        myGLCD.printNumF(stemp,2, RIGHT, 110);
      }
      else if(item==menuItem3){
        myGLCD.print("o",240, position-8);
        myGLCD.print("C)",255, position);
        myGLCD.setColor(VGA_GREEN);
        myGLCD.printNumF(ptemp,2, RIGHT, 150);
        }
      else if(item==menuItem4){
        myGLCD.setColor(VGA_RED);
        myGLCD.printNumI(stime, RIGHT, 190);
      }
      else if (item==menuItem5){
        myGLCD.setColor(VGA_GREEN);
        myGLCD.printNumI(ptime, RIGHT, 230);
      }
      else if (item==menuItem1){
        if (status==1){
          myGLCD.setColor(VGA_RED);
        }
        else{
          myGLCD.setColor(VGA_GREEN);
        }
        myGLCD.print(item, LEFT, 70);
      }
    }   
}
void systemRun() {
  digitalWrite(relay1Pin, LOW);
  digitalWrite(relay2Pin, LOW);
  digitalWrite(relay3Pin, HIGH);
  // Reset all variables
  m = 0;
  counter = 0;
  ptime = 0;
  }
void systemStop() {
  digitalWrite(relay1Pin, HIGH);
  digitalWrite(relay2Pin, HIGH);
  digitalWrite(relay3Pin, LOW);// Relay 3 is ON
  // Reset all variables
  counter = 0;
  ptime = 0;
  }
void systemReset(){
  stemp=100.00;
  stime=30;
}
void timerIsr() {
  encoder->service();
}
void readRotaryEncoder()
{
  value += encoder->getValue();
  
  if (value > last) {
    last = value;
    up = true;
    }
  else   if (value < last) {
    last = value;
    down = true;
  }
  ClickEncoder::Button b = encoder->getButton();
   if (b != ClickEncoder::Open) {
   switch (b) {
      case ClickEncoder::Clicked:
         middle=true;
        break;
    }
  }    
}

// Function to turn off all relays
void turnOffAllRelays() {
  digitalWrite(relay1Pin, HIGH);
  digitalWrite(relay2Pin, HIGH);
  digitalWrite(relay3Pin, HIGH);
  digitalWrite(relay4Pin, HIGH);
}

  void manageRelay2State() {
    if (m == 1) {
      counter++;
      if (counter <= tOn) {
        digitalWrite(relay2Pin, LOW); // Turn Relay 2 ON for tOn
      } else if (counter > tOn && counter <= tOn + tOff) {
        digitalWrite(relay2Pin, HIGH); // Turn Relay 2 OFF for tOff
      } else {
        if (m == 1) { // If m is still 1 after tOff, reset the counter
          counter = 0;
        }
      }
    } else {
      digitalWrite(relay3Pin, HIGH); // Turn Relay 3 OFF if m is not 1
    }

  }

  void manageRelay3State() {
    ptime++;
    if (ptime == stime && status==true) {
      systemStop(); // Stop operation when t reaches 0
    }
}