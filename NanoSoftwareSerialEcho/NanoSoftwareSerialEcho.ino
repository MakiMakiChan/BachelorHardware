#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
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

void loop() { // run over and over
  if (mySerial.available()) {
    char gelesen = mySerial.read();
    if(gelesen == 'a')  mySerial.write('0');
    else if(gelesen == 'b')  mySerial.write('1');
    else mySerial.write('x');
  }
}

