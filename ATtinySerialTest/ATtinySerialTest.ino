#include <SoftwareSerial.h>

#define rxPin 3
#define txPin 4
char ID = 0;

SoftwareSerial port = SoftwareSerial(rxPin, txPin);
void setup() {
  // put your setup code here, to run once:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);

  port.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  port.write(ID);
  delay(1000);
}
