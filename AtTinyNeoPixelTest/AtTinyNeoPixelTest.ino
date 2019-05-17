#include <Adafruit_NeoPixel.h>

#define PIN 2
Adafruit_NeoPixel strip = Adafruit_NeoPixel(5, PIN, NEO_RGB + NEO_KHZ800);

uint32_t presetColors[] = {0x140000, 0x001400, 0x000014, 0x141400, 0x140014, 0x000000};
int totalPresetColors = (sizeof(presetColors) / sizeof(uint32_t));
void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

}

void loop() {
  //looping();
  setIndividual(0,presetColors[0]);
  delay(500);
  setIndividual(4,presetColors[3]);
  delay(500);
  setIndividual(1,presetColors[4]);
  delay(500);
  setIndividual(3,presetColors[1]);
  delay(500);
  setIndividual(2,presetColors[2]);
  delay(500);
  colorWipe((presetColors[5]), 50); // Off
  
}

void setIndividual(uint16_t pixel, uint32_t color) {
  strip.setPixelColor(pixel, color);
  strip.show();
}

void looping () {
  // Some example procedures showing how to display to the pixels:
  colorWipe((presetColors[0]), 50); // Red
  colorWipe((presetColors[1]), 50); // Green
  colorWipe((presetColors[2]), 50); // Blue
  colorWipe((presetColors[3]), 50); // Yellow
  colorWipe((presetColors[4]), 50); // Purple
  colorWipe((presetColors[5]), 50); // Off
  delay(500);
}
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

