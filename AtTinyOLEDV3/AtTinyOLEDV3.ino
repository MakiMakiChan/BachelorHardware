//
// Needs to run at 1MHz to make it more stable
// otherweise the display might freez
//

#include <SoftwareSerial.h>
#include "TinyWireM.h"
#include "TinyOzOLED.h"

SoftwareSerial mySerial(3, 4); // RX, TX

char ID = '0';
char data = 0;
byte posP[2][4] = {{5, 7, 9, 11}, {0, 0, 0, 0}}; //Position of Positionnumbers
long numP[4] = {1, 2, 3, 4};
byte posC[2][4] = {{5, 7, 9, 11}, {1, 2, 3, 4}}; //Position of Chipnumbers
const int numLocations = 4;
long locations[numLocations];


void setup() {
  pinMode(1, INPUT); //Localization
  mySerial.begin(9600);


  OzOled.init();
  OzOled.clearDisplay();           //clear the screen and set start position to top left corner
  OzOled.setNormalDisplay();       //Set display to Normal mode
  OzOled.sendCommand(0xA1);        // set Orientation
  OzOled.sendCommand(0xC8);

  displayStatic();
}

void loop() {
  //respond to localization querry
  if (digitalRead(1) == LOW) {
    mySerial.print(ID);
    delay(100);
    if (mySerial.available())
    {
      int trashData = mySerial.read();
    }
  }


  char gelesen;
  if (mySerial.available()) {
    gelesen = mySerial.read();
    delay(25);
    if (mySerial.available()) {
       data = mySerial.read();  
    }else {
      data = 'x';
    }
   
  }
  if (gelesen == ID) {
    delay(100);

    int availableData = 0;
    while (availableData != 4) {
      availableData = mySerial.available();
    }
    //get information for which chip is at which port
    locations[0] = (int) data - 48;
    for (int i = 1; i < numLocations; i++) {
      locations[i] =  (int) mySerial.read() - 48;
    }

    //refresh static information
    displayStatic();

    //display dynamic information
    for (int i = 0; i < numLocations; i++) {
      if (locations[i] != 0) {
      OzOled.printNumber(locations[i], posC[0][i], posC[1][0]);
      }
   //   mySerial.write(ID);
   //   mySerial.write(locations[i]);
    }
    

    OzOled.setCursorXY(0, 7);
    char mode = mySerial.read();
    if (mode == '0') {
      OzOled.printString("Hard coded");
    } else {
      OzOled.printString("Pass on");
    }
   // mySerial.write(ID);
   // mySerial.write(mode);
   // mySerial.write(ID);
   // mySerial.write('Y');
  }
}

void displayStatic() {
  OzOled.clearDisplay();
  OzOled.printString("Pos: ");
  OzOled.setCursorXY(0, 1);
  OzOled.printString("Chip:");
  OzOled.setCursorXY(0, 6);
  OzOled.printString("Mode:");
  for (byte i = 0; i < numLocations; i++) {
    OzOled.printNumber(numP[i], posP[0][i], posP[1][0]);
  }
}

