#include<TinyWireM.h>

#define address 0x20
#define ddrA 0x00
#define ddrB 0x01
#define portA 0x12
#define portB 0x13

void setup() {
  TinyWireM.begin();
  delay(500);
  TinyWireM.beginTransmission(address);
  TinyWireM.write(ddrA);   //DDRA adress (DDRB adress 0x01)
  TinyWireM.write(0x00);   //make all pins OUTPUTs
  TinyWireM.endTransmission();

  TinyWireM.beginTransmission(address);
  TinyWireM.write(ddrB);   //DDRA adress (DDRB adress 0x01)
  TinyWireM.write(0x00);   //make all pins OUTPUTs
  TinyWireM.endTransmission();
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
      return 0b01110111; //0
      break;
    case 1:
      return 0b00000110; //1
      break;
    case 2:
      return 0b11010011; //2
      break;
    case 3:
      return 0b10010111; //3
      break;
    case 4:
      return 0b10100110; //4
      break;
    case 5:
      return 0b10110101; //5
      break;
    case 6:
      return 0b11110101; //6
      break;
    case 7:
      return 0b00000111; //7
      break;
    case 8:
      return 0b11110111; //8
      break;
    case 9:
      return 0b10110111; //9
      break;
  }
}
void loop() {
  for (int i = 0; i < 10; i++) {
   writeToChip(portA, i);
   for(int j = 0; j < 10; j++) {
    writeToChip(portB, j);
    delay(200);
   }
  }
  writeToChip(portA, -1);
  writeToChip(portB, -1);
    delay(500);
}
