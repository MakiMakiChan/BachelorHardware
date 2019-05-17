#include "TinyWireM.h"
#include "TinyOzOLED.h"
byte posP[2][4] = {{5, 7, 9, 11}, {0, 0, 0, 0}};
long numP[4] = {1, 2, 3, 4};
byte posC[2][4] = {{5, 7, 9, 11}, {1, 1, 1, 1}};
long numC[4] = {2, 6, 5, 1};


void setup() {
  OzOled.init();
  OzOled.clearDisplay();           //clear the screen and set start position to top left corner
  OzOled.setNormalDisplay();       //Set display to Normal mode

  OzOled.sendCommand(0xA1);        // set Orientation
  OzOled.sendCommand(0xC8);

  OzOled.printString("Pos: ");

  //OzOled.setCursorXY(posX,posY);
  OzOled.printNumber(numP[0], posP[0][0], posP[1][0]);
  OzOled.printNumber(numP[1], posP[0][1], posP[1][0]);
  OzOled.printNumber(numP[2], posP[0][2], posP[1][0]);
  OzOled.printNumber(numP[3], posP[0][3], posP[1][0]);

  OzOled.printNumber(numC[0], posC[0][0], posC[1][0]);
  OzOled.printNumber(numC[1], posC[0][1], posC[1][0]);
  OzOled.printNumber(numC[2], posC[0][2], posC[1][0]);
  OzOled.printNumber(numC[3], posC[0][3], posC[1][0]);

  OzOled.setCursorXY(0, 1);
  OzOled.printString("Chip:");
  OzOled.setCursorXY(0,6);
  OzOled.printString("Mode:");
  OzOled.setCursorXY(0,7);
  OzOled.printString("Hard coded");
}

void loop() {
  // put your main code here, to run repeatedly: }
}

