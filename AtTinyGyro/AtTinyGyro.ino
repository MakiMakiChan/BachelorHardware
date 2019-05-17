#include<TinyWireM.h>
#include<SoftwareSerial.h>

SoftwareSerial Serial2 (3,4); //(Rx,Tx)
const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, GyX, GyY, GyZ;
double timeStep, time, timePrev;
double ary, gry, gsy, ry;

int i;
double gyroScale = 131;


void setup() {
  TinyWireM.begin();
  TinyWireM.beginTransmission(MPU_addr);
  TinyWireM.write(0x6B);  // PWR_MGMT_1 register
  TinyWireM.write(0);     // set to zero (wakes up the MPU-6050)
  TinyWireM.endTransmission();
  
 

  Serial2.begin(9600);
  time = millis();
  i = 1;
}
void loop() {
  char a = 70;
  char b = 25;
  Serial2.print(a);
  delay(1000);
  Serial2.print(b);
  delay(1000);
}
void loopy() {

  // set up time for integration
  timePrev = time;
  time = millis();
  timeStep = (time - timePrev) / 1000; // time-step in s

  //get data from MPU6050
  TinyWireM.beginTransmission(MPU_addr);
  TinyWireM.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  TinyWireM.endTransmission();
  TinyWireM.requestFrom(MPU_addr, 6); // request a total of 6 registers
  AcX = TinyWireM.read() << 8 | TinyWireM.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = TinyWireM.read() << 8 | TinyWireM.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = TinyWireM.read() << 8 | TinyWireM.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)

  TinyWireM.beginTransmission(MPU_addr);
  TinyWireM.write(0x45);  // starting with register 0x45 (GYRO_YOUT_H)
  TinyWireM.endTransmission();
  TinyWireM.requestFrom(MPU_addr, 2); // request a total of 2 registers
  GyY = TinyWireM.read() << 8 | TinyWireM.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)

  
  // apply gyro scale from datasheet
  gsy = GyY / gyroScale;

  // calculate accelerometer angles
  ary = (180 / 3.141592) * atan(AcY / sqrt(square(AcX) + square(AcZ)));
  
  // set initial values equal to accel values
  if (i == 1) {
    gry = ary;
  }
  // integrate to find the gyro angle
  else {
    gry = gry + (timeStep * gsy);
  }

  // apply filter
  ry = (0.96 * ary) + (0.04 * gry);
  char data = map(abs(ry),0,86,0,90);
  // print result
  Serial2.print(data);
  i = i + 1;
  delay(100);
}
