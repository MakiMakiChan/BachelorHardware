#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>

#define PIN 2

SoftwareSerial mySerial(3, 4); // RX, TX
Adafruit_NeoPixel strip = Adafruit_NeoPixel(5, PIN, NEO_RGB + NEO_KHZ800);

char ID = '0';
int confLevel = 0;
char data = 0;
uint32_t presetColors[] = {0x000000, 0x140000, 0x001400, 0x000014, 0x141400, 0x140014};
int totalPresetColors = (sizeof(presetColors) / sizeof(uint32_t));
const int numLocations = 4;
int locations[numLocations];
int mode = 0;

void setup() {
  pinMode(1, INPUT); //Localization
  strip.begin();
  strip.show();
  setIndividual(1, presetColors[1]);
  delay(500);
  clearAll();
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
  char gelesen;
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
      delay(100);
      locations[0] = (int) data - 48;
      for (int i = 1; i < numLocations; i++) {
        locations[i] = mySerial.read() - 48;
      }
      for (uint16_t i = 0; i < 4; i++) {
        if (locations[i] ==0 ||locations[i] ==1 ||locations[i] ==2 ||locations[i] == 3 ||locations[i] ==4) {
          setIndividual(i+1, presetColors[locations[i]]);
          mySerial.write(ID);
          mySerial.write(locations[i]);
        }

      }
      mySerial.write(ID);
      mySerial.write("Y");
    }
  }


}



void setIndividual(uint16_t pixel, uint32_t color) {
  strip.setPixelColor(pixel, color);
  strip.show();
}

void clearAll() {
  for (uint16_t pixel = 0; pixel <= 5; pixel++)  {
    strip.setPixelColor(pixel, 0x000000);
  }
  strip.show();
}

