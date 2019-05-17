/*
  This example program can be used to test the "SoftwareSerialWithHalfDuplex" library, adapted from the SoftwareSerial library.
  The value of half-duplex is that one pin can be used for both transmitting and receiving data.
  Also many devices can be daisy-chained to the same line. RS485 still commonly uses half-duplex.

  By default the library works the same as the SoftwareSerial library, but by adding a couple of additional arguments
  it can be configured for half-duplex. In that case, the transmit pin is set by default to an input, with the pull-up set.
  When transmitting the pin temporarily switches to an output until the byte is sent, then flips back to input. When a module is
  receiving it should not be able to transmit, and vice-versa. This library probably won't work as is if you need inverted-logic.

  To use this test example, upload SoftwareSerialWithHalfDuplex_test_partA to as many arduinos as you like. Be sure to change
  "myID" for each arduino loaded with partA. Upload SoftwareSerialWithHalfDuplex_test_partB to a different arduino. All arduinos
  should be connected to each other by the same communications pin, and by ground. Open up the serial monitor pointing to partB.
  When you type in the id number of one of the devices it should respond.

  This is a first draft of the library and test programs. It appears to work, but has only been tested on a limited basis,
  and hasn't yet been tested with any native half-duplex devices (like the bioloid ax12 robot servo).
  Seems fairly reliable up to 57600 baud. As with all serial neither error checking, nor addressing are implemented,
  so it is likely that you will need to do this yourself. Also, you can make use of other protocols such as i2c.
  I am looking for any feedback, advice and help at this stage.
  Contact me at n.stedman@steddyrobots.com or on the arduino forums.
*/



#include <SoftwareSerialWithHalfDuplex.h>

#define comPin 2

// add arguments of false (for inverse_logic?), false (for full-duplex?) for half duplex.
// you can then use the same pin for both transmit and receive.
SoftwareSerialWithHalfDuplex port(comPin, comPin, false, false);


char pose[4];
char last;
int wait = 200;
int conf = 50;
int testPressed = 0;


void setup() {
  // Input Pins
  pinMode(13, INPUT);
  
  // Lacalization pins
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(12, LOW);
  digitalWrite(11, LOW);
  digitalWrite(10, LOW);
  digitalWrite(9, LOW);

  port.begin(9600);         // to connect with other devices
  Serial.begin(9600);            // to see who's connected
  Serial.println("Chip da");
}

void loop() {
  //check whether button is pressed
  //check where cpis are
  boolean testButton = digitalRead(13); 
  if ( testButton == LOW) {
    testPressed ++;  
    if (testPressed == conf){
      checkPins();
    }
  }else {
    testPressed = 0;
  }

  //check if communication has arrived
  if (port.available() > 0) {
    int chip = port.read();
    delay(10);
    //Serial.print ("Chip: ");
    //Serial.print (chip);
    if (port.available() > 0) {
      int data = port.read();
     // Serial.print (" Data: ");
     // Serial.println(data);
      port.write(2);
      port.write(data);
    }
  }
}


void checkPins() {
  Serial.println("checking");
  digitalWrite(12, HIGH);
  delay(wait);
  check(0);
  digitalWrite(12, LOW);
  delay(wait);
  digitalWrite(11, HIGH);
  delay(wait);
  check(1);
  digitalWrite(11, LOW);
  delay(wait);
  digitalWrite(10, HIGH);
  delay(wait);
  check(2);
  digitalWrite(10, LOW);
  delay(wait);
  digitalWrite(9, HIGH);
  delay(wait);
  check(3);
  digitalWrite(9, LOW);
  delay(wait);
  for (int i = 0; i < 4; i++) {
    Serial.print("An Pin ");
    Serial.print(12-i);
    Serial.print(" ist Chip : ");
    Serial.println(pose[i]);
  }
  Serial.println(" ");

}
void check(int i) {
  switch (i) {
    case 0: Serial.print("Pin 12 ");
      break;
    case 1: Serial.print("Pin 11 ");
      break;
    case 2: Serial.print("Pin 10 ");
      break;
    case 3: Serial.print("Pin 9 ");
      break;
  }
  if (port.available()) {
    char c = port.read();
    pose[i] = c;
    Serial.println(c);
    while(port.available()> 0) {
      int discard = port.read();
    }
  }
  else {
    pose[i] = 'x';
    Serial.println("-");
  }

}

