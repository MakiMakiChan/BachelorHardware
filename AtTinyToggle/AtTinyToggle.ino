#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 4); // RX, TX

char ID = '2';
int confLevel = 0;
char dataOut = 0;
boolean lastState = false;

void setup() {
  pinMode(1, INPUT); //Localization
  pinMode(2, INPUT); //Button
  digitalWrite(2, HIGH);
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
  char gelesen, dataIn;
  if (mySerial.available()) {
    gelesen = mySerial.read();
    delay(25);
    if (mySerial.available()) {
      dataIn = mySerial.read();
    }
    else {
      dataIn = 'x';
    }
    if (gelesen == '0') {
      for (int i = 0; i < 4 ;i++) {
        char trashData = mySerial.read();
      }
    }
  }

  //respond to Button press
  if (digitalRead(2) == LOW) {
    confLevel++;
    if (confLevel == 50 && lastState == false) {
      dataOut = 255;
      mySerial.write(ID);
      mySerial.write(dataOut);
      lastState = true;
    } else if (confLevel == 50 && lastState == true) {
      dataOut = 0;
      mySerial.write(ID);
      mySerial.write(dataOut);
      lastState = false;
    }
    
  } else if (confLevel != 50) {  //Button unsuccsessfully pressen / Bounce
    confLevel = 0;    //reset confLevel
  } 
}


