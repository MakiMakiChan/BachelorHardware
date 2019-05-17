#include <TinyWireM.h>
#include <SoftwareSerial.h>

SoftwareSerial Serial2 (3, 4);
const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, GyX, GyY, GyZ;
double timeStep, time, timePrev;
double ary, gry, gsy, ry;
boolean state = LOW;

void setup() {
  pinMode(1, OUTPUT);

  TinyWireM.begin();
  TinyWireM.beginTransmission(MPU_addr);
  TinyWireM.write(0x6B);  // PWR_MGMT_1 register
  TinyWireM.write(0);     // set to zero (wakes up the MPU-6050)
  TinyWireM.endTransmission();

  digitalWrite(1, HIGH);
  delay(1000);
  time = millis();
  Serial2.begin(9600);
}

void loop() {
  for ( char i = 0; i < 255; i++) {
    TinyWireM.beginTransmission(MPU_addr);
    TinyWireM.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
    TinyWireM.endTransmission();
    uint8_t check = TinyWireM.requestFrom(MPU_addr, 1); // request a total of 6 registers

    AcX = TinyWireM.read() << 8; // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)

    TinyWireM.beginTransmission(MPU_addr);
    TinyWireM.write(0x3C);  // starting with register 0x3B (ACCEL_XOUT_H)
    TinyWireM.endTransmission();
    TinyWireM.requestFrom(MPU_addr, 1); // request a total of 6 registers

    AcX = AcX | TinyWireM.read();
    Serial2.print(i);
    Serial2.print(check);
    //  Serial2.print(map(AcX,0,65535,0,255));
    delay(1000);

    if (millis() - time > 1000) {
      digitalWrite(1, state);
      if (state == LOW) state = HIGH;
      else state =  LOW;
      time = millis();
    }
  }
}
