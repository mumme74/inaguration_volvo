// arduino MEGA 2560
#include <Arduino.h>
#include <stdint.h>
#include <FastLED.h> // http://fastled.io/
#include <MemoryFree.h>
#include "Leds.h"


const uint8_t DATAOUT_CH1 = 3, // as in arduino i/o pin
              DATAOUT_CH2 = 4,
              DATAOUT_CH3 = 5,
              DATAOUT_CH4 = 6,
              DATAOUT_CH5 = 7;

const uint8_t OUT_CH1 = 9,  // open drain 40A 100V
              OUT_CH2 = 8;

const uint8_t INPUT_CH1 = 54, //A0,
              INPUT_CH2 = 55, //A2,
              INPUT_CH3 = 56; //A1;


const uint8_t NUM_LEDS_CH1 = 150,
              NUM_LEDS_CH2 = 150,
              NUM_LEDS_CH3 = 150,
              NUM_LEDS_CH4 = 150;

CRGB leds_ch1[NUM_LEDS_CH1],
     leds_ch2[NUM_LEDS_CH2],
     leds_ch3[NUM_LEDS_CH3],
     leds_ch4[NUM_LEDS_CH4];

SegmentPart kuLogoLeft(nullptr, 10, 30),
            kuLogoMiddle(nullptr, 42, 20),
            kuLogoRight(nullptr, 75, 40);

Segment kuLogo();


// https://forum.arduino.cc/index.php?topic=483504.0

void clear();
void setAllToColor(uint32_t col);
void snake(uint32_t bgColor, uint32_t snakeColor, int fromLed, int toLed, int waitTime);

void setup() {
  delay(100); // allow to setup
  FastLED.addLeds<UCS1903, DATAOUT_CH1, BRG>(leds_ch1, NUM_LEDS_CH1);
  FastLED.addLeds<UCS1903, DATAOUT_CH2, BRG>(leds_ch2, NUM_LEDS_CH2);
  FastLED.addLeds<UCS1903, DATAOUT_CH3, BRG>(leds_ch3, NUM_LEDS_CH3);
  FastLED.addLeds<UCS1903, DATAOUT_CH4, BRG>(leds_ch4, NUM_LEDS_CH4);

  // setup controller
  CLEDController *cont_ch1 = &FastLED[0];
  kuLogoLeft.setLedController(cont_ch1);
  kuLogoMiddle.setLedController(cont_ch1);
  kuLogoMiddle.setLedController(cont_ch1);

  // kulogo
}

void loop() {
  static unsigned long secondsTick = 0;
  if (secondsTick < millis()) {
    secondsTick = millis() + 1000;
    Serial.print("mem:"); Serial.println(freeMemory());
  }


   // Move a single white led
   
   for(int led = 0; led < NUM_LEDS_CH1; ++led) {
      // Turn our current led on to white, then show the leds
      leds_ch1[led] = CRGB::White;

      // Show the leds (only one of which is set to white, from above)
      FastLED.show();

      // Wait a little bit
      delay(100);

      // Turn our current led back to black for the next loop around
      leds_ch1[led] = CRGB::DarkRed;
   }

   for (int times = 0; times < 3; ++times) {
      setAllToColor(CRGB::DarkGray);
      delay(200);
      setAllToColor(CRGB::DarkBlue);
      delay(150);
      setAllToColor(CRGB::DarkGreen);
      delay(100);
   }

   snake(CRGB::Indigo, CRGB::Lavender, 0, NUM_LEDS_CH1, 40);

   fadeToBlackBy(leds_ch1, NUM_LEDS_CH1, 20);

   fill_gradient_RGB(leds_ch1, 2, CRGB::DarkGray, 30, CRGB::Indigo);

   clear();
   delay(500);
}

void snake(uint32_t bgColor, uint32_t snakeColor, int fromLed, int toLed, int waitTime) {
  // set background
  for (int led = fromLed; led < toLed; ++led) {
    leds_ch1[led] = bgColor;
  }

  int previous = -1;
  for (int led = fromLed; led < toLed + 1; ++led) {
    if (led < toLed)
      leds_ch1[led] = snakeColor;
    if (previous > -1)
      leds_ch1[previous] = bgColor;
    previous = led;
    FastLED.show();
    delay(waitTime);
  }
}

void setAllToColor(uint32_t col) {
   for (int led = 0; led < NUM_LEDS_CH1; ++led)
      leds_ch1[led] = col;
   FastLED.show();
  
}

void clear(){
  // clear
  setAllToColor(CRGB::Black);
}



