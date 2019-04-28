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
              OUT_CH2 = 8,
              OUT_LED = 13;

const uint8_t INPUT_CH1 = 54, //A0,
              INPUT_CH2 = 56, //A2,
              INPUT_CH3 = 55; //A1;


              // Bah!! 3LED are in parallell in these cheap LED strips
const uint8_t NUM_LEDS_CH1 = 150 / 3,
              NUM_LEDS_CH2 = 150 / 3,
              NUM_LEDS_CH3 = 150 / 3,
              NUM_LEDS_CH4 = 150 / 3;

CRGB leds_ch1[NUM_LEDS_CH1],
     leds_ch2[NUM_LEDS_CH2],
     leds_ch3[NUM_LEDS_CH3],
     leds_ch4[NUM_LEDS_CH4];

CLEDController
     *ch1 = &FastLED.addLeds<UCS1903, DATAOUT_CH1, BRG>(leds_ch1, NUM_LEDS_CH1),
     *ch2 = &FastLED.addLeds<UCS1903, DATAOUT_CH2, BRG>(leds_ch2, NUM_LEDS_CH2),
     *ch3 = &FastLED.addLeds<UCS1903, DATAOUT_CH3, BRG>(leds_ch3, NUM_LEDS_CH3),
     *ch4 = &FastLED.addLeds<UCS1903, DATAOUT_CH4, BRG>(leds_ch4, NUM_LEDS_CH4);


SegmentPart kuLogoPart1(ch1, 0, 5),
            kuLogoPart2(ch1, 7, 4),
            kuLogoPart3(ch1, 13, 10),
            connectStripPartKuLogo(ch1, 24, 25),
            liljasLogoInsidePart(ch3, 0, 18),
            liljasLogoOutsidePart(ch3, 18, 19),
            connectStripPartLiljasLogo(ch3, 39, 11),
            volvoLogoOutsidePart1(ch2, 0, 6),
            volvoLogoOutsidePart2(ch2, 20, 6),
            volvoLogoInsideRingPart1(ch2, 6, 4),
            volvoLogoInsideRingPart2(ch2, 16, 4),
            volvoLogoTextPart(ch2, 10, 6);
            //connectStripPart3(ch2, 96, 9);

Segment kuLogoLeft,
        kuLogoCenter,
        kuLogoRight,
        liljasLogoInside,
        liljasLogoOutside,
        volvoLogoInside,
        volvoLogoText,
        volvoLogoOutside,
        connectStrip;

SegmentCompound kuLogo, liljasLogo, volvoLogo, all;



// our main program goes in here
void FastLED_Action::program() {
  // kulogo
  kuLogoLeft.addSegmentPart(kuLogoPart1);
  kuLogoCenter.addSegmentPart(kuLogoPart2);
  kuLogoRight.addSegmentPart(kuLogoPart3);
  kuLogo.addSegment(kuLogoLeft);
  kuLogo.addSegment(kuLogoCenter);
  kuLogo.addSegment(kuLogoRight);


  // liljas logo
  liljasLogoInside.addSegmentPart(liljasLogoInsidePart);
  liljasLogoOutside.addSegmentPart(liljasLogoOutsidePart);
  liljasLogo.addSegment(liljasLogoInside);
  liljasLogo.addSegment(liljasLogoOutside);

  // volvo logo
  volvoLogoInside.addSegmentPart(volvoLogoInsideRingPart1);
  volvoLogoInside.addSegmentPart(volvoLogoInsideRingPart2);
  volvoLogoText.addSegmentPart(volvoLogoTextPart);
  volvoLogoOutside.addSegmentPart(volvoLogoOutsidePart1);
  volvoLogoOutside.addSegmentPart(volvoLogoOutsidePart2);



  // connectStrip
  connectStrip.addSegmentPart(connectStripPartKuLogo);
  connectStrip.addSegmentPart(connectStripPartLiljasLogo);
  //connectStrip.addSegmentPart(connectStripPart3);

  // actions
  ActionSnake actSnake1(CRGB::Red, CRGB::White),
              actSnake2(CRGB::Indigo, CRGB::Lavender),
              actSnake3(CRGB::Lavender, CRGB::Indigo, true, true);
  ActionColor actColorDarkGray(CRGB::DarkGray),
              actColorDarkBlue(CRGB::DarkBlue),
              actColorDarkGreen(CRGB::DarkGreen);
  ActionFade actFade(5);
  ActionColorLadder actLadder(CRGB::DarkGray, CRGB::Indigo);

  // DEBUGG!!!
  ActionColor actRed(CRGB::Red),
              actGreen(CRGB::Green),
              actBlue(CRGB::Blue),
              actOlive(CRGB::Olive),
              actNavy(CRGB::Navy),
              actSilver(CRGB::Silver),
              actGray(CRGB::Green),
              actOrange(CRGB::OrangeRed);
  kuLogo.addAction(actRed);
  connectStrip.addAction(actGreen);

  liljasLogo.addAction(actBlue);
  liljasLogo.addAction(actOlive);

  volvoLogoInside.addAction(actSilver);
  volvoLogoText.addAction(actOrange);
  volvoLogoOutside.addAction(actGray);

  //kuLogo.addAction(actBlue);
  //kuLogo.addAction(actSnake1);
  Serial.println("before");
  kuLogo.yieldUntilAction(10);
  Serial.println("after 10times");
  return;
// end debug

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

  // apparently mega2560 doesn't have pulldown
  pinMode(INPUT_CH1, INPUT);
  pinMode(INPUT_CH2, INPUT);

  pinMode(OUT_CH1, OUTPUT);
  pinMode(OUT_CH2, OUTPUT);
  pinMode(OUT_LED, OUTPUT);

  // clear led controllers
  ch1->clearLeds(NUM_LEDS_CH1);
  ch2->clearLeds(NUM_LEDS_CH2);
  ch3->clearLeds(NUM_LEDS_CH3);
  ch4->clearLeds(NUM_LEDS_CH4);
  FastLED.show();
}


// main loop
void loop() {
  // sanity check, so we don't run our head straight through our stack
  static uint32_t secondsTick = 0;
  int mem = freeMemory();
  if (secondsTick < millis() || mem > 6144) {
    secondsTick = millis() + 1000;
    Serial.print("mem:"); Serial.println(freeMemory());
  }

  static uint32_t programRunned = 0,
                  ledTick = 0;
  static bool programRunning = false;

  if (ledTick < millis()) {
    ledTick = millis() + (programRunning ? 300 : 1000);
    // toggle led
    digitalWrite(OUT_LED, !digitalRead(OUT_LED));
  }

  FastLED_Action::loop();
  if (!programRunning ) {
  //if (digitalRead(INPUT_CH1) && digitalRead(INPUT_CH2) && !programRunning) {
    programRunning = true;
    FastLED_Action::runProgram(2);
    programRunning = false;
    programRunned = millis();
  }

  if (programRunned > 0 && programRunned + 120000 < millis())
    programRunned = 0; // restart!
}




