//Repiter

#include <SoftwareSerial.h>
SoftwareSerial serialIn(2, 3);


void setup() {
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
  pinMode(13, OUTPUT);
  serialIn.begin(9600);
  serialIn.listen();
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);

}

void loop() {
  char c;
  while (serialIn.available() == 0) {
    ;
  }
  while (serialIn.available() > 0) {
    c = serialIn.read();
    if (c == 'a')
      digitalWrite(13, HIGH);
    else
      digitalWrite(13, LOW);
  }
  serialIn.print(c);
}
