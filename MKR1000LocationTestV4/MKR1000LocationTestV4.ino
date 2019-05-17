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

char mode = '0';
int confLvl = 0;

void setup()
{
  static unsigned char count = 0;

  //declariren der LED, Localisations Pins und Button
  pinMode(MKR1000_LED, OUTPUT);
  pinMode(2, OUTPUT);   //Zeile 1
  pinMode(3, OUTPUT);   //Zeile 2
  pinMode(4, OUTPUT);   //Spalte 1
  pinMode(5, OUTPUT);   //Spalte 2
  pinMode(A1, INPUT_PULLUP);    //Button für mode Change

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

  checkLocations();
  Serial.println("Setup Complete");
}

/*
   aktiviert eine Location für den Location-Check
*/
void activateLocation(int zeile, int spalte) {
  digitalWrite(zeile, HIGH);
  digitalWrite(spalte, HIGH);
}
/*
   deaktiviert die Location nach einem Location-Checks
*/
void deactivateLocation(int zeile, int spalte) {
  digitalWrite(zeile, LOW);
  digitalWrite(spalte, LOW);
}
/*
   checkt ob sich an einer bestimmten Stelle ein Chip befindet
*/
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

  //Gib Array über Chip 0 aus
  Serial2.write('0');
  for (int i = 0; i < zeilen; i++) {
    for (int j = 0; j < spalten; j++) {
      switch (locations[i][j]) {
        case '1':
          Serial2.write('1');
          break;
        case '2':
          Serial2.write('2');
          break;
        case '3':
          Serial2.write('3');
          break;
        case '4':
          Serial2.write('4');
          break;
        default:
          Serial2.write('0');
          break;
      }
    }
  }
  Serial2.write(mode);
}
/*
   checkt ob der Button gedrückt wurde
   handelt bouncing
*/
void checkButton() {
  if (digitalRead(A1) == 0) {
    confLvl++;
    if (confLvl == 50) {
      changeMode();
      checkLocations();
    }
  } else {
    confLvl = 0;
  }
}
/*
   ändert den Modus mit dem Nachrichten weitergeleitet werden
*/
void changeMode() {
  if (mode == '0') {
    mode = '1'; //change to pass on
  } else {
    mode = '0'; //return to hardcoded
  }
}
/*
   checkt ob Daten über Serial reingekommen sind und leitet die an die Chips weiter
*/
void checkSerial() {
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
}
/*
      checkt ob Date von den Chips gekommen sind
      wenn ja leite sie über Serial weiter
      und checke von welchem Chip sie kommen, addressiere sie an den passenden neuen Chip
*/
void checkSerial2() {
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
    Serial.println(gelesenData, DEC);

    if (mode == '0') {
      hardCoded(gelesenChip, gelesenData);
    } else {
      passOn(gelesenChip, gelesenData);
    }
  }

}
/*
   reicht Nachrichten von einer Position zur Nächten weiter
*/
void passOn(char sender, char data) {
  int sLocation = getLocation(sender);
  if (sLocation == -1) {
    Serial.println("nicht gefunden");
    checkLocations();
    sLocation = getLocation(sender);
    if (sLocation == -1) {
      return;
    }
  }  
  char target = getTarget(sLocation + 1);
  if (target != 'x') {
    Serial2.write(target);
    Serial2.write(data);
  }
}
/*
   findet die Position eines Chips auf dem Brett
   sofern der Chip bereits localisiert wurde
   sonst gibt -1 zurück
*/
int getLocation(char sender) {
  int location = 0;
  for (int i = 0; i < zeilen; i++) {
    for (int j = 0; j < spalten; j++) {
      if (sender != locations[i][j]) {
        location++;
      } else {
        return location;
      }
    }
  }
  return -1;
}
/*
 * findet den Chip für den die weitergeleitete Nachricht gedacht ist
 */
char getTarget(int location) {
  if(location >= zeilen + spalten){
    return'x';
  }
  int i, j;
  if (location < zeilen) {
    i = 0;
    j = location;
  } else {
    i = 1;
    j = location - 2;
  }
  return locations[i][j];
}
/*
   leitet Daten nach fest vorgegebenem Muster weiter
*/
void hardCoded(char sender, char data) {
  switch (sender) {
    case '1':
      Serial2.write('4');
      Serial2.write(data);
      break;
    case '2':
      Serial2.write('1');
      Serial2.write(data);
      Serial2.write('3');
      Serial2.write(data);
      break;
  }
}
/*
     checkt die Locations alle 5 Sekunden ob sich was geändert hat
     lässt auserdem die onboard LED im gleichen Takt blinken
*/
void checkTime() {
  if (millis() - ms > 5000)
  {
    ms = millis();
    digitalWrite(MKR1000_LED, state);
    state = !state;
    checkLocations();
  }
}


void loop()
{
  checkButton();

  checkSerial();
  checkSerial2();

  checkTime(); //also checks the location

}


