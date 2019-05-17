#include <TinyWireM.h>
#include <SoftwareSerial.h>

SoftwareSerial Serial2 (3,4);
const int MPU_addr = 0x68;
int16_t AcX;


void setup() {
  TinyWireM.begin();
  TinyWireM.beginTransmission(MPU_addr);
  TinyWireM.write(0x6B);
  TinyWireM.write(0);
  TinyWireM.endTransmission();

  Serial2.begin(9600);

}

void loop() {
  TinyWireM.beginTransmission(MPU_addr);
  TinyWireM.write(0x3B);
  TinyWireM.endTransmission();
  TinyWireM.requestFrom(MPU_addr, 1);

  AcX = TinyWireM.read();

  Serial2.print(AcX);
  delay(1000);
}
