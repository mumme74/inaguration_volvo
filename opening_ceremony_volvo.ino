// arduino MEGA 2560
#include <Arduino.h>
#include <stdint.h>
#include <FastLED.h> // http://fastled.io/
#include <MemoryFree.h>
#include <FastLED_Action.h>


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

CLEDController
     *ch1 = &FastLED.addLeds<UCS1903, DATAOUT_CH1, BRG>(leds_ch1, NUM_LEDS_CH1),
     *ch2 = &FastLED.addLeds<UCS1903, DATAOUT_CH2, BRG>(leds_ch2, NUM_LEDS_CH2),
     *ch3 = &FastLED.addLeds<UCS1903, DATAOUT_CH3, BRG>(leds_ch3, NUM_LEDS_CH3),
     *ch4 = &FastLED.addLeds<UCS1903, DATAOUT_CH4, BRG>(leds_ch4, NUM_LEDS_CH4);


SegmentPart kuLogoPart1(ch1, 10, 45),
            kuLogoPart2(ch1, 60, 35),
            kuLogoPart3(ch1, 105, 45),
            connectStripPart1(ch1, 0, 10),
            connectStripPart2(ch1, 56, 4),
            connectStripPart3(ch1, 96, 9);

Segment kuLogo,
        liljasLogo,
        volvoLogo,
        connectStrip;

SegmentCompound all;



// our main program goes in here
void FastLED_Action::program() {

  // setup kuLogoParts
  kuLogoPart1.setLedController(ch1);
  kuLogoPart2.setLedController(ch1);
  kuLogoPart3.setLedController(ch1);

  // kulogo
  kuLogo.addSegmentPart(kuLogoPart1);
  kuLogo.addSegmentPart(kuLogoPart2);
  kuLogo.addSegmentPart(kuLogoPart3);

  // connectStrip
  connectStrip.addSegmentPart(connectStripPart1);
  connectStrip.addSegmentPart(connectStripPart2);
  connectStrip.addSegmentPart(connectStripPart3);

  // actions
  ActionSnake actSnake1(CRGB::Red, CRGB::White),
              actSnake2(CRGB::Indigo, CRGB::Lavender),
              actSnake3(CRGB::Lavender, CRGB::Indigo, true, true);
  ActionColor actColorDarkGray(CRGB::DarkGray),
              actColorDarkBlue(CRGB::DarkBlue),
              actColorDarkGreen(CRGB::DarkGreen);
  ActionFade actFade(5);
  ActionColorLadder actLadder(CRGB::DarkGray, CRGB::Indigo);

  // setup actions
  kuLogo.addAction(actSnake1);
  kuLogo.addAction(actColorDarkGray);
  kuLogo.addAction(actColorDarkBlue);
  kuLogo.addAction(actColorDarkGreen);
  kuLogo.addAction(actSnake2);
  kuLogo.addAction(actSnake3);
  kuLogo.addAction(actFade);
  kuLogo.addAction(actLadder);

  // let the actions play out, repeat 5 times
  for (uint8_t i = 0; i < 2; ++i)
    kuLogo.yieldUntilAction(kuLogo.actionsSize());

  // blow our load of glitter (steer airpressure vales)
  FastLED_Action::clearAllActions();
  ActionGotoColor actDark1(CRGB::Gray, CRGB::Black, 300),
                  actWhite1(CRGB::Black, CRGB::WhiteSmoke, 200);
  kuLogo.addAction(actDark1);
  kuLogo.addAction(actWhite1);
  kuLogo.yieldUntilAction(actWhite1);
  digitalWrite(OUT_CH1, HIGH);
  kuLogo.yieldUntilAction(actDark1);
  digitalWrite(OUT_CH2, HIGH);
  kuLogo.yieldUntilAction(2);
  digitalWrite(OUT_CH1, LOW);
  digitalWrite(OUT_CH2, LOW);

}

void setup() {
  delay(100); // allow to setup
  Serial.begin(115200);

  // FIXME need to solder 2 pulldown resistors to our inputs
  // apparently mega2560 doesn't have pulldown
  pinMode(INPUT_CH1, INPUT);
  pinMode(INPUT_CH2, INPUT);

  pinMode(OUT_CH1, OUTPUT);
  pinMode(OUT_CH2, OUTPUT);
}


// main loop
void loop() {
  // sanity check, so we don't run our head straight through our stack
  static unsigned long secondsTick = 0;
  int mem = freeMemory();
  if (secondsTick < millis() || mem > 6144) {
    secondsTick = millis() + 1000;
    Serial.print("mem:"); Serial.println(freeMemory());
  }

  static uint32_t programRunned = 0;

  FastLED_Action::loop();
  if (digitalRead(INPUT_CH1) && digitalRead(INPUT_CH2)) {
    FastLED_Action::runProgram(false);
    programRunned = millis();
  }

  if (programRunned > 0 && programRunned + 120000 < millis())
    programRunned = 0; // restart!
}




