#include <SoftwareSerial.h>
SoftwareSerial serialIn(2,3);
SoftwareSerial serialOut(3, 2);

void setup() {
  Serial.begin(9600);
  Serial.println("Chip da!");
  pinMode(2,INPUT);
  serialIn.begin(9600);
 
}

void loop() {
 int c;
 while(serialIn.available() == 0) {
  ;
 }
 while(serialIn.available() > 0){
 c = serialIn.read();
 }
 Serial.println(c);
}
