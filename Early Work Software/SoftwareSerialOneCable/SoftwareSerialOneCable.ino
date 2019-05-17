//Master

#include <SoftwareSerial.h>
SoftwareSerial serialIn(2, 2);
SoftwareSerial noo(5,5);

void setup() {
  Serial.begin(9600);
  Serial.println("Chip da!");
  pinMode(2, INPUT);
  serialIn.begin(9600);
  noo.begin(9600);

}

void loop() {
  char c;
  while (Serial.available() == 0) {
    ;
  }
  while (Serial.available() > 0) {
    c = Serial.read();
  }
  Serial.print("Eingegeben: ");
  Serial.println(c);
  noo.listen();
  pinMode(2, OUTPUT);
  serialIn.print(c);
  delay(100);
  pinMode(2, INPUT);
  serialIn.listen();
  delay(100);
  char d;
  while (serialIn.available () == 0) {
    ;
  }
  while (serialIn.available() > 0) {
    d = serialIn.read();
  }
  Serial.print("Char read: ");
  Serial.println(d);
}
