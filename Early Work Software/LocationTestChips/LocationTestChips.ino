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

#define comPin 0

SoftwareSerialWithHalfDuplex port(comPin,comPin,false,false);

const int myID = 1; // 1 Button , 2 LED
boolean lastState = LOW;
boolean buttonState = LOW;
int conf = 50;
int testPressed = 0;
int wait = 1000;

void setup(){
  pinMode(1, INPUT); // Communication prompting Pin
  pinMode(3, INPUT); // Button Pin
  pinMode(4, OUTPUT); // led pin
  port.begin(9600);
}

void loop(){
  //checking if mainboard is checking for position
  boolean state = digitalRead(1);
    if (state == HIGH && lastState == LOW){
      port.println(myID);
      lastState = HIGH;
    } else if(state == LOW && lastState == HIGH) {
      lastState = LOW; 
  }

 //checking button 
  boolean testButton = digitalRead(3); 
  if ( testButton == LOW && buttonState == LOW) {
    testPressed ++;  
    if (testPressed == conf){
      port.write(myID);
      port.write(255);
      buttonState = HIGH;
    }
  }else {
    testPressed = 0;
  }
  if ( buttonState == HIGH && testButton == HIGH) {
    byte zero = 0;
    port.write(myID);
    port.write(zero);
    buttonState = LOW; 
  }
/*
  // checking serial port for infos for led
  if ( port.available() > 0) {
    if ( port.read() == myID) {
      delay(10);
      if ( port.available() > 0) {
        int data = port.read();
        if ( data > 0) {
          digitalWrite(4, HIGH);
        }else {
          digitalWrite(4, LOW);
        }
      }
    }
  }
  */
}

