#include <SoftwareSerial.h>

SoftwareSerial Serial2 (8,11);

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  // put your setup code here, to run once:
  Serial.println("HUHU");
}

void loop() {
  if(Serial2.available()){
    while(Serial2.available()) {
      char gelesen = Serial2.read();
      Serial.println(gelesen);
      delay(25);
    }
  }
  if(Serial.available()) {
    while(Serial.available()) {
      delay(5);
      Serial2.write(Serial.read());
    }
  }
}
