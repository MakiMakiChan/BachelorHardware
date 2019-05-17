
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


void setup()
{
  static unsigned char count = 0;

  pinMode(MKR1000_LED, OUTPUT);

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

  digitalWrite(MKR1000_LED, HIGH);

  Serial.println("Setup Complete");
}

void loop()
{


  if (Serial.available())
  {
    String gelesen;
    while (Serial.available() > 0) {
      char c = Serial.read();
      gelesen += c;
    }
    Serial.print(gelesen);
    Serial2.print(gelesen);
  }
  if (Serial2.available())
  {
    String gelesen2;
    while (Serial2.available() > 0) {
      char c = Serial2.read();
      gelesen2 += c;
    }
    Serial.print(gelesen2);
    Serial2.print(gelesen2);
  }


  if (millis() - ms > 100)
  {
    ms = millis();
    digitalWrite(MKR1000_LED, state);
    state = !state;
  }

}
