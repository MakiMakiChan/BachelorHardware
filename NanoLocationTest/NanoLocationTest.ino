#include <SoftwareSerial.h>

SoftwareSerial mySerial(8, 11); // RX, TX

char ID = '1';


void setup() {
  //Definiere Localisations Pins
  pinMode(4, INPUT);
  pinMode(5, INPUT);

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  Serial.println("SoftwareSerial Initialised");
}

void loop() {
  if (digitalRead(5) == LOW)
  {
    mySerial.print(ID);
    Serial.println("*");
    delay(100);
    if (mySerial.available())
    { 
      Serial.println("#");
      int trashData = mySerial.read();
    }
  }
  char gelesen, data;
  if (mySerial.available()) {
    Serial.print("Empfangen: ");
    gelesen = mySerial.read();
    Serial.print("ID: ");
    Serial.print(gelesen);
    delay(25);
    if (mySerial.available()) {
      data = mySerial.read();
      Serial.print(" Data: ");
      Serial.print(data);
    }
    else {
      data = 'x';
    }

    if (gelesen == ID) {
      Serial.println("     ID erkannt, sende Antwort");
      mySerial.write(ID);
      mySerial.write(data);
    } else {
      Serial.println("     Nicht meine ID");
    }
  }

}


