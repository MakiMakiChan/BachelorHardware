#include <SoftwareSerial.h>

SoftwareSerial mySerial(1, 2); // RX, TX

char ID = '3';


void setup() {

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
}

void loop() { // run over and over
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
      mySerial.write(data);
    }
  }

}


