#include <Wire.h>
#include "Adafruit_MCP23017.h"

Adafruit_MCP23017 mcp;

void setup() {
  mcp.begin();      // use default address 0
  //Serial.begin(9600);
  for (int i = 0; i < 8; i++) {
    mcp.pinMode(i, OUTPUT);
  }
  mcp.writeGPIOAB(0x0004);
  delay(1000);

}


uint16_t convert(char input) {
  switch (input) {
    case 0:
      return 0b0000000001110111; //0
      break;
    case 1:
      return 0b00000000000000110; //1
      break;
    case 2:
      return 0b0000000011010011; //2
      break;
    case 3:
      return 0b0000000010010111; //3
      break;
    case 4:
      return 0b0000000010100110; //4
      break;
    case 5:
      return 0b0000000010110101; //5
      break;
    case 6:
      return 0b0000000011110101; //6
      break;
    case 7:
      return 0b0000000000000111; //7
      break;
    case 8:
      return 0b0000000011110111; //8
      break;
    case 9:
      return 0b0000000010110111; //9
      break;
  }
}

void loop() {
  int data = 0;
  //if (Serial.available()) {
  //  while (Serial.available()) {
  //     data = Serial.read() - 48;
  //  }
  //  Serial.println(data);
  mcp.writeGPIOAB(convert(data));
  delay(500);
  mcp.writeGPIOAB(convert(0));
  //  }


}

/*
  mcp.writeGPIOAB(0b0000000001110111);//0
  delay(1000);
  mcp.writeGPIOAB(0x0000);
  delay(100);
  mcp.writeGPIOAB(0b00000000000000110);//1
  delay(1000);
  mcp.writeGPIOAB(0x0000);
  delay(100);
  mcp.writeGPIOAB(0b0000000011010011);//2
  delay(1000);
  mcp.writeGPIOAB(0x0000);
  delay(100);
  mcp.writeGPIOAB(0b0000000010010111);//3
  delay(1000);
  mcp.writeGPIOAB(0x0000);
  delay(100);
  mcp.writeGPIOAB(0b0000000010100110);//4
  delay(1000);
  mcp.writeGPIOAB(0x0000);
  delay(100);
  mcp.writeGPIOAB(0b0000000010110101);//5
  delay(1000);
  mcp.writeGPIOAB(0x0000);
  delay(100);
  mcp.writeGPIOAB(0b0000000011110101);//6
  delay(1000);
  mcp.writeGPIOAB(0x0000);
  delay(100);
  mcp.writeGPIOAB(0b0000000000000111);//7
  delay(1000);
  mcp.writeGPIOAB(0x0000);
  delay(100);
  mcp.writeGPIOAB(0b0000000011110111);//8
  delay(1000);
  mcp.writeGPIOAB(0x0000);
  delay(100);
  mcp.writeGPIOAB(0b0000000010110111);//9
  delay(1000);
  mcp.writeGPIOAB(0x0000);
  delay(100);
*/
