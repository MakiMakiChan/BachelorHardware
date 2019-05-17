#include <SoftwareSerial.h>

SoftwareSerial mySerial(3,4); // RX, TX

char ID = '3';


void setup() {
  pinMode(1,INPUT);
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
}

void loop() { 
  if(digitalRead(1) == LOW){
    mySerial.print(ID);
    delay(100);
    if (mySerial.available())
    { 
      int trashData = mySerial.read();
    }
  }
  char gelesen, data;
  if (mySerial.available()) {
    gelesen = mySerial.read();
    delay(25);
    if (mySerial.available()) {
      data = mySerial.read();
    }
    else {
      data = 'x';
    }
    
    if (gelesen == ID) {
      mySerial.write(ID);
      mySerial.write(data);
    }
  }

}


