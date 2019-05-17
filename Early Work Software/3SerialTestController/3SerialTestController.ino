//Controller

#include <SoftwareSerial.h>
SoftwareSerial serialIn(2, 3);
SoftwareSerial serialOut(3, 2);

void setup() {
  Serial.begin(9600);
  pinMode(2,INPUT);
  serialIn.begin(9600);
  serialOut.begin(9600);
}

void loop() {
  char c = 'z';
  int i = 0;
  Serial.print("Bitte Buchstaben eingeben:");
  while (Serial.available() == 0) {
    ;
  }

  c = Serial.read();
  Serial.println(c);
  Serial.print("Bitte Zahl eingeben:");
  while (Serial.available() == 0) {
    ;
  }
  i = Serial.read();
  i-=48;
  Serial.println(i);
  if (c != 'z') {
    
    pinMode(2,OUTPUT);
    serialOut.write(c);
    serialOut.write(i);
    pinMode(2,INPUT);
    
  }
  serialIn.listen();
  Serial.println("Auf Antwort warten ");
  while(serialIn.available() == 0){
    ;
  }
  Serial.print("Antwort erhalten: ");
  uint8_t antwort = serialIn.read();
  Serial.println(antwort);
}
