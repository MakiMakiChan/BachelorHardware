#include <SoftwareSerial.h>
#include<TinyWireM.h>

#define address 0x20
#define ddrA 0x00
#define ddrB 0x01
#define portA 0x12
#define portB 0x13

SoftwareSerial mySerial(3, 4); // RX, TX

static boolean entered = false;

void setup() {
  TinyWireM.begin();
  TinyWireM.beginTransmission(address);
  TinyWireM.write(ddrA);   //DDRA adress (DDRB adress 0x01)
  TinyWireM.write(0x00);   //make all pins OUTPUTs
  TinyWireM.endTransmission();

  TinyWireM.beginTransmission(address);
  TinyWireM.write(ddrB);   //DDRA adress (DDRB adress 0x01)
  TinyWireM.write(0x00);   //make all pins OUTPUTs
  TinyWireM.endTransmission();

  writeToChip(portB, 0);
  mySerial.begin(9600);
}

void writeToChip(byte reg, int number) {
  TinyWireM.beginTransmission(address);
  TinyWireM.write(reg);   //PORTA adress (PORTB 0x13)
  TinyWireM.write(convert(number));   //
  TinyWireM.endTransmission();

}
uint8_t convert(int input) {
  switch (input) {
    case -1:
      return 0x00; //komplett aus
      break;
    case 0:
      return 0b00111111; //0
      break;
    case 1:
      return 0b00000110; //1
      break;
    case 2:
      return 0b01011011; //2
      break;
    case 3:
      return 0b01001111; //3
      break;
    case 4:
      return 0b01100110; //4
      break;
    case 5:
      return 0b01101101; //5
      break;
    case 6:
      return 0b01111101; //6
      break;
    case 7:
      return 0b00000111; //7
      break;
    case 8:
      return 0b01111111; //8
      break;
    case 9:
      return 0b01101111; //9
      break;
  }
}

void loop() {
  if (mySerial.available()) {
    char dataIn = mySerial.read();
    int data = (int) dataIn;
    writeToChip(portA, data / 10);
    writeToChip(portB, data % 10);

  }
}
