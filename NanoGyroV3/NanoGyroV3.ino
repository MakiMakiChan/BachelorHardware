#include<Wire.h>
#include<SoftwareSerial.h>

SoftwareSerial Serial2 (8, 9); //(Rx,Tx)
const char ID = '1';
boolean onOff = false;
const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, GyX, GyY, GyZ;
double timeStep, time, timePrev, timeSend;
double ary, gry, gsy, ry;
int i;
double gyroScale = 131;


void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  Serial.begin(9600);
  Serial.println("Hello World");
  Serial2.begin(9600);

  pinMode(5, INPUT);

  time = millis();
  i = 1;
  timeSend = time;
}

void loop() {
  //respond to localization request
  if (digitalRead(5) == LOW)
  {
    Serial2.print(ID);
    delay(100);
    if (Serial2.available())
    {
      // Serial.println("#");
      int trashData = Serial2.read();
    }
    delay(100);
  }

  //handle incoming data
  unsigned char gelesen, dataIn;
  if (Serial2.available()) {
    gelesen = Serial2.read();
    delay(50);
    if (Serial2.available()) {
      dataIn = Serial2.read();
      //   Serial.print(" Data: ");
      //   Serial.print(dataIn,DEC);
    }
    else {
      dataIn = 'x';
    }

    if (gelesen == '0') {
      Serial.println("delete");
      for (int i = 0; i < 5 ; i++) {
        char trashData = Serial2.read();
      }
    }
    if (gelesen == ID) {
      Serial.println("     ID erkannt, sende Antwort");
       Serial.println(gelesen);
       Serial.println(dataIn);
      if (dataIn == 255) {
        onOff = true;
      } else if (dataIn == 0){
        onOff = false;
      }
      Serial.println(onOff);
    }
  }


  // set up time for integration
  timePrev = time;
  time = millis();
  timeStep = (time - timePrev) / 1000; // time-step in s

  //get data from MPU6050
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 6, true); // request a total of 6 registers
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)

  Wire.beginTransmission(MPU_addr);
  Wire.write(0x45);  // starting with register 0x45 (GYRO_YOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 2, true); // request a total of 2 registers
  GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)


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
  char data = map(abs(ry), 0, 86, 0, 90);

  if (onOff == true) {
    // only runs about 2 times per second
    if (time - timeSend > 437) {
      // send data
      //Serial.print(data, BIN);
      //Serial.print(" ");
      //Serial.println(data, DEC);
      Serial2.write(ID);
      Serial2.write(data);

      timeSend = time;

      i = i + 1;
    }
  }
}
