#include <SoftwareSerial.h>

#define rxPin 2
#define txPin 3
char ID = 0;

SoftwareSerial port = SoftwareSerial(rxPin, txPin);
void setup() {
  // put your setup code here, to run once:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);

  port.begin(9600);
  Serial.begin(9600);
  delay(100);
  Serial.println("Chip da");
  
}

void loop() {
  delay(950);
 byte got = 5;
 Serial.println("trying");
 if(port.available() >0)
  {
    Serial.println("Reading");
     got = port.read();
     Serial.println(got);
  }
 
}
