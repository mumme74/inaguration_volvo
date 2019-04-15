#include "Arduino.h"
#include "FastLED.h" // http://fastled.io/
#define NUM_LEDS 50
#define DATA_PIN 3

CRGB leds[NUM_LEDS];
//#define CLOCK_PIN 8


// https://forum.arduino.cc/index.php?topic=483504.0

void clear();
void setAllToColor(uint32_t col);
void snake(uint32_t bgColor, uint32_t snakeColor, int fromLed, int toLed, int waitTime);

void setup() {
  delay(100);
  FastLED.addLeds<UCS1903, DATA_PIN, BRG>(leds, NUM_LEDS); 
}

void loop() {
   // Move a single white led
   
   for(int led = 0; led < NUM_LEDS; ++led) {
      // Turn our current led on to white, then show the leds
      leds[led] = CRGB::White;

      // Show the leds (only one of which is set to white, from above)
      FastLED.show();

      // Wait a little bit
      delay(100);

      // Turn our current led back to black for the next loop around
      leds[led] = CRGB::DarkRed;
   }

   for (int times = 0; times < 3; ++times) {
      setAllToColor(CRGB::DarkGray);
      delay(200);
      setAllToColor(CRGB::DarkBlue);
      delay(150);
      setAllToColor(CRGB::DarkGreen);
      delay(100);
   }

   snake(CRGB::Indigo, CRGB::Lavender, 0, NUM_LEDS, 40);

   fadeToBlackBy(leds, NUM_LEDS, 20);

   fill_gradient_RGB(leds, 2, CRGB::DarkGray, 30, CRGB::Indigo);

   clear();
   delay(500);
}

void snake(uint32_t bgColor, uint32_t snakeColor, int fromLed, int toLed, int waitTime) {
  // set background
  for (int led = fromLed; led < toLed; ++led) {
    leds[led] = bgColor;
  }

  int previous = -1;
  for (int led = fromLed; led < toLed + 1; ++led) {
    if (led < toLed)
      leds[led] = snakeColor;
    if (previous > -1)
      leds[previous] = bgColor;
    previous = led;
    FastLED.show();
    delay(waitTime);
  }
}

void setAllToColor(uint32_t col) {
   for (int led = 0; led < NUM_LEDS; ++led)
      leds[led] = col;
   FastLED.show();
  
}

void clear(){
  // clear
  setAllToColor(CRGB::Black);
}



