#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 4); // RX, TX

char ID = '3';
int confLevel = 0;
char data = 0;
void setup() {
  pinMode(1, INPUT); //Localization
  pinMode(2, OUTPUT); //LED
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
}

void loop() {

  //respond to localization querry
  if (digitalRead(1) == LOW) {
    mySerial.print(ID);
    delay(100);
    if (mySerial.available())
    {
      int trashData = mySerial.read();
    }
  }

  //read Data from Serial
  char gelesen;
  if (mySerial.available()) {
    gelesen = mySerial.read();
    delay(25);
    if (mySerial.available()) {
      data = mySerial.read();
    }
    else {
      data = 'x';
    }
    if (gelesen == '0') {
      for (int i = 0; i < 4 ;i++) {
        char trashData = mySerial.read();
      }
    }
  }
  
  if (gelesen == ID) {
    digitalWrite(2,data);
   // mySerial.write(ID);
   // mySerial.write(data);
  }
}


