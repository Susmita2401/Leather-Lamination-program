#include <UTFT.h>
#include <ClickEncoder.h>
#include <TimerOne.h>

const int encoderPinA = 2;  // CLK or A phase pin
const int encoderPinB = 3;  // DT or B phase pin
const int encoderSwitchPin = 4;  // SW or switch pin

// Declare which fonts we will be using
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
static boolean status = true;
static double stemp=100.00;
double ptemp=50.00;
static int stime = 30;
int ptime = 15;

boolean up = false;
boolean down = false;
boolean middle = false;

ClickEncoder *encoder;
int16_t last, value;

void setup()
{
  Serial.begin(9600);
  // Setup the LCD
  myGLCD.InitLCD();
  myGLCD.setFont(GroteskBold16x32);
  myGLCD.clrScr();
  myGLCD.fillScr(VGA_WHITE);

  pinMode(encoderPinA,INPUT);
  pinMode(encoderPinB,INPUT);
  pinMode(encoderSwitchPin,INPUT);
    
  encoder = new ClickEncoder(encoderPinA, encoderPinB, encoderSwitchPin);
  encoder->setAccelerationEnabled(false);
   
  Timer1.initialize(100);
  Timer1.attachInterrupt(timerIsr); 
  
  last = encoder->getValue();
}

void loop()
{
  menu();
  readRotaryEncoder();
     
  if (up && page==1){
    up=false;
    menuitem++;
    if (menuitem>6){
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
    if (menuitem<1){
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
        status=false;
        menuItem1="> STOP";
        systemRun();
      }
      else{
        status=true;
        menuItem1="> RUN ";
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
  
  void menu(){
      
      myGLCD.setBackColor(VGA_NAVY);
      myGLCD.setColor(VGA_WHITE);
      myGLCD.print("Leather Lamination Machine", CENTER, 2);
      myGLCD.setColor(VGA_NAVY);
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
      myGLCD.setBackColor(VGA_NAVY);
       myGLCD.setColor(VGA_WHITE);
      if (item==menuItem1){
        myGLCD.print(item, LEFT, 70);
      }
      if (item==menuItem2){
        if (page==1){
        myGLCD.print(item, 2, position);
        myGLCD.print("o",180, position-8);
        myGLCD.print("C)",195, position);
        myGLCD.setBackColor(VGA_WHITE);
        myGLCD.setColor(VGA_RED);
        myGLCD.printNumF(stemp,2, RIGHT, 110);
        }
        else if (page==2){
          myGLCD.setBackColor(VGA_WHITE);
          myGLCD.setColor(VGA_BLACK);
          myGLCD.print(item, 2, position);
          myGLCD.print("o",180, position-8);
          myGLCD.print("C)",195, position);
          myGLCD.setBackColor(VGA_NAVY);
          myGLCD.setColor(VGA_WHITE);
          myGLCD.printNumF(stemp,2, RIGHT, 110);
        }
      }
      else if(item==menuItem3){
        myGLCD.print(item, 2, position);
        myGLCD.print("o",240, position-8);
        myGLCD.print("C)",255, position);
        myGLCD.setColor(VGA_GREEN);
        myGLCD.setBackColor(VGA_WHITE);
        myGLCD.printNumF(ptemp,2, RIGHT, 150);
        }
      else if(item==menuItem4){
        if (page==1){
        myGLCD.print(item, 2, position);
        myGLCD.setColor(VGA_RED);
        myGLCD.setBackColor(VGA_WHITE);
        myGLCD.printNumI(stime, RIGHT, 190);
        }
        else if (page==2){
          myGLCD.setColor(VGA_BLACK);
          myGLCD.setBackColor(VGA_WHITE);
          myGLCD.print(item, 2, position);
          myGLCD.setColor(VGA_WHITE);
          myGLCD.setBackColor(VGA_NAVY);
          myGLCD.printNumI(stime, RIGHT, 190);
        }
      }
      else if (item==menuItem5){
        myGLCD.print(item, 2, position);
        myGLCD.setColor(VGA_GREEN);
        myGLCD.setBackColor(VGA_WHITE);
        myGLCD.printNumI(ptime, RIGHT, 230);
      }
      else if (item==menuItem6){
        myGLCD.print(item, 2, position);
      }
    }
    else
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
        if (status==true){
          myGLCD.setColor(VGA_GREEN);
        }
        else{
          myGLCD.setColor(VGA_RED);
        }
        myGLCD.print(item, LEFT, 70);
      }
    }
  }
void systemRun(){
  // Function to run system
  Serial.print("System is Running");
}
void systemStop(){
  // Function to stop system
  Serial.print("System has Stopped");
}
void systemReset(){
  stemp=100.00;
  stime = 30;
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

