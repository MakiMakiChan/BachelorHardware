
/*
  SERCOM Test

  Test the ability to add extra hardware serial ports to the MKR1000
  This sketch has the following serial interfaces:
    Serial  - Native USB interface
    Serial1 - Default serial port on D13, D14 (Sercom 5)
    Serial2 - Extra serial port on D0, D1 (Sercom 3)
    Serial3 - Extra serial port on D4, D5 (Sercom 4)

  Good explanation of sercom funcationality here:
  https://learn.adafruit.com/using-atsamd21-sercom-to-add-more-spi-i2c-serial-ports/muxing-it-up
  This sketch will echo characters recieved on any of the 4 serial ports to all other serial ports.
  for Arduino MKR1000
  by Tom Kuehn
  26/06/2016
*/

#include <Arduino.h>                              // required before wiring_private.h
#include <wiring_private.h>


/*
                        Extra Hardware Serial Port
*/
// Serial2 pin and pad definitions (in Arduino files Variant.h & Variant.cpp)
#define PIN_SERIAL2_RX       (1ul)                // Pin description number for PIO_SERCOM on D1
#define PIN_SERIAL2_TX       (0ul)                // Pin description number for PIO_SERCOM on D0
#define PAD_SERIAL2_TX       (UART_TX_PAD_0)      // SERCOM pad 0 TX
#define PAD_SERIAL2_RX       (SERCOM_RX_PAD_1)    // SERCOM pad 1 RX

// Instantiate the extra Serial classes
Uart Serial2(&sercom3, PIN_SERIAL2_RX, PIN_SERIAL2_TX, PAD_SERIAL2_RX, PAD_SERIAL2_TX);

void SERCOM3_Handler()    // Interrupt handler for SERCOM3
{
  Serial2.IrqHandler();
}


/*
                Declarations
*/

static const char MKR1000_LED       = 6;
static unsigned long ms = 0;
static boolean state = HIGH;
static unsigned char c = 0;
static const int zeilen = 2;
static const int spalten = 2;


char locations[zeilen][spalten];

void setup()
{
  static unsigned char count = 0;

  //declariren der LED und Localisations Pins
  pinMode(MKR1000_LED, OUTPUT);
  pinMode(2, OUTPUT);   //Zeile 1
  pinMode(3, OUTPUT);   //Zeile 2
  pinMode(4, OUTPUT);   //Spalte 1
  pinMode(5, OUTPUT);   //Spalte 2


  // Start Serial for debugging on the Serial Monitor
  Serial.begin(9600);
  while (!Serial && (count < 30) )
  {
    delay(200);     // Wait for serial port to connect with timeout. Needed for native USB
    digitalWrite(MKR1000_LED, state);
    state = !state;
    count++;
  }
 

  pinPeripheral(0, PIO_SERCOM);   // Assign pins 0 & 1 SERCOM functionality
  pinPeripheral(1, PIO_SERCOM);
  Serial2.begin(9600);           // Begin Serial2

  //initialisiren der Output Pins
  digitalWrite(MKR1000_LED, HIGH);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);

  Serial.println("Setup Complete");
}


void activateLocation(int zeile, int spalte) {
  digitalWrite(zeile, HIGH);
  digitalWrite(spalte, HIGH);
}
void deactivateLocation(int zeile, int spalte) {
  digitalWrite(zeile, LOW);
  digitalWrite(spalte, LOW);
}

void checkLocations() {
  //Testet jede Location
  Serial.println("Location Test");
  for (int i = 2; i < 2 + zeilen; i++) {
    for (int j = 4; j < 4 + spalten; j++) {
      activateLocation(i, j);
      delay(50);
      //Wenn eine Antwort kommt speichere sie im locations Array
      if (Serial2.available()) {
        locations[i - 2][j - 4] = Serial2.read();
      } else {
        locations[i - 2][j - 4] = 'x';
      }
      deactivateLocation(i, j);
    }
  }
  //Gebe Array aus
  for (int i = 0; i < zeilen; i++) {
    for (int j = 0; j < spalten; j++) {
      Serial.print(locations[i][j]);
      Serial.print(" ");
    }
    Serial.println(" ");
  }

}

void loop()
{


  if (Serial.available())
  {
    char c1, c2;
    c1 = Serial.read();
    if (c1 == 't') {
      checkLocations();
    } else  {
      c2 = Serial.read();

      Serial.print("Senden an Chip: ");
      Serial.print(c1);
      Serial.print(" sende: ");
      Serial.print(c2);
      Serial2.write(c1);
      Serial2.write(c2);
    }
    delay(100); //gebe Zeit zum Antworten
  }
  if (Serial2.available())
  {
    Serial.print("    Empfange von Chip: ");
    char gelesenChip = Serial2.read();
    char gelesenData;
    delay(10);
    if (Serial2.available() > 0) {
      gelesenData = Serial2.read();

    } else {
      gelesenData = 'x';
    }
    Serial.print(gelesenChip);
    Serial.print(" Data: ");
    Serial.println(gelesenData,DEC);
    
    switch (gelesenChip) {
      case '1':
        Serial2.write('4');
        Serial2.write(gelesenData);
        break;
      case '2':
        Serial2.write('1');
        Serial2.write(gelesenData);
        Serial2.write('3');
        Serial2.write(gelesenData);
        break;
    }
  }

  if (millis() - ms > 5000)
  {
    ms = millis();
    digitalWrite(MKR1000_LED, state);
    state = !state;
    checkLocations();
  }

}


