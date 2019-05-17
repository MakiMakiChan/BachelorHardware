//Controller
int anzahl =5;
int durchlauf = 0;
#include <SoftwareSerial.h>
SoftwareSerial serialIn(2, 3);
SoftwareSerial serialOut(3, 2);

void setup() {
  Serial.begin(9600);
 
 
  serialOut.begin(9600);
  pinMode(2,OUTPUT);
}

void loop() {
  int i = 5;
  
  delay(1000);
  while (durchlauf < anzahl) {
  serialOut.write('a'); 
  durchlauf++;
  }
  delay(5000);
  serialOut.write(i);
}
