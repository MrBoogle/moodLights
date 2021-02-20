#include <FastLED.h>

#include<IRLibRecv.h>

IRrecv MyReceiver(11);
//Createthereceiver.Usepin2
#include<IRLibDecodeBase.h>
#include<IRLib_P01_NEC.h>
#include<IRLibCombo.h>
IRdecode MyDecoder;

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few 
// of the kinds of animation patterns you can quickly and easily 
// compose using FastLED.  
//
// This example also shows one easy way to define multiple 
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    3
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    141
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120

void setup() {
  delay(3000); // 3 second delay for recovery
  Serial.begin(9600);
  MyReceiver.enableIRIn();//startreceiving
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();

SimplePatternList gPatterns = {defaultMode, lightning, rain, snow, clearMode, ambient};

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
  
void loop()
{

  if(MyReceiver.getResults()){
      //waittillitreturnstrue
      MyDecoder.decode();
      //MyDecoder.dumpResults();
      Serial.println(MyDecoder.value);
      MyReceiver.enableIRIn();
      //restartthereceiver
      }

  delay(50);
  /*Serial.println(analogRead(A0));
  Serial.println(analogRead(A1));
  Serial.println(analogRead(A2));
  Serial.println(analogRead(A3));
  Serial.println("---");*/
  //Could use map for better code but arduino barely has any memory
  // > 600 means pin is high
  if (analogRead(A0) < 600 && analogRead(A1) < 600 && analogRead(A2) < 600 && analogRead(A3) == 0) {
    gCurrentPatternNumber = 0;
    } 
  if (analogRead(A0) < 600 && analogRead(A1) < 600 && analogRead(A2) < 600 && analogRead(A3) > 600) {
    gCurrentPatternNumber = 1;
    } 
  if (analogRead(A0) < 600 && analogRead(A1) < 600 && analogRead(A2) > 600 && analogRead(A3) < 600) {
    gCurrentPatternNumber = 2;
    } 
  if (analogRead(A0) < 600 && analogRead(A1) < 600 && analogRead(A2) > 600 && analogRead(A3) > 600) {
    gCurrentPatternNumber = 3;
    } 
  if (analogRead(A0) < 600 && analogRead(A1) > 600 && analogRead(A2) < 600 && analogRead(A3) == 0) {
    gCurrentPatternNumber = 4;
    } 
  if (analogRead(A0) < 600 && analogRead(A1) > 600 && analogRead(A2) < 600 && analogRead(A3) > 600) {
    gCurrentPatternNumber = 5;
    } 

  
  
  
  
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  //EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
  
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void defaultMode() {
  sinelon();
  }



void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rain()
{
  for (int i = 110; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Gray;
    }
    for (int i = 0; i < 110; i++) {
    leds[i] = CRGB::Black;
    }
    
    //96, 80, 64, 48, 32, 16, 0
  
    for (int i = 96; i >= 0; i-=16) { 
    leds[i+2] = leds[i+4] = leds[i+6] = leds[i+8] = leds[i+12] = leds[i+10] = leds[i+14] = leds[i] = CRGB::Blue;
    //leds[i+2] = leds[i+4] = leds[i+6] = leds[i+8]
    FastLED.delay(200);
    if (i != 0) {
    leds[i+2-16] = leds[i+4-16] = leds[i+6-16] = leds[i+8-16] = leds[i-16] = leds[i+12-16] = leds[i+10-16] = leds[i+14-16] = CRGB::Blue;}
    leds[i+2] = leds[i+4] = leds[i+6] = leds[i+8] = leds[i+12] = leds[i+10] = leds[i+14]= leds[i] = CRGB::Black;
    FastLED.delay(200);
    }
    
}

void lightning () {
  rain();
  leds[96] = leds[80] = leds[64] = leds[49] = leds[34] = leds[18] = leds[2] = CRGB::White;
      FastLED.delay(150);
      leds[96] = leds[80] = leds[64] = leds[48] = leds[34] = leds[18] = leds[2] = CRGB::Black;
      FastLED.delay(50);
      leds[96] = leds[80] = leds[64] = leds[48] = leds[34] = leds[18] = leds[2] = CRGB::White;
      FastLED.delay(150);
      leds[96] = leds[80] = leds[64] = leds[48] = leds[34] = leds[18] = leds[2] = CRGB::Black;
  }


void snow () {
  int positions[7];
  int fullRotation = 141-127;

    //Set all leds to blue
   for (int i= 0; i < NUM_LEDS; i++) {
     leds[i] = CRGB(0, 0, 255);
   }

  for (int i= 0; i < fullRotation/2; i++) {
     positions[i] = fullRotation*random() + 127;
     leds[ positions[i] ];
     //led[ positions[i] ]
  }

  

  while (true) {
    for (int i = 0; i < fullRotation/2; i++) {
      positions[i]  -= fullRotation;
      if (positions[i] < 0) {
        positions[i] = -1;
        goto loopBreak;
      }
      leds[ positions[i] ] = CRGB(235, 235, 255);
    }

   FastLED.delay(5);
  }

  loopBreak: return;
  
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

#define Holly_Green 0x00580c
#define Holly_Red   0xB00402
const TProgmemRGBPalette16 Holly_p FL_PROGMEM =
{  Holly_Green, Holly_Green, Holly_Green, Holly_Green, 
   Holly_Green, Holly_Green, Holly_Green, Holly_Green, 
   Holly_Green, Holly_Green, Holly_Green, Holly_Green, 
   Holly_Green, Holly_Green, Holly_Green, Holly_Red 
};

void ambient() {
  CRGBPalette16 palette = Holly_p;
   for (int i= 0; i < NUM_LEDS; i++) {
    //No clue what this'll due
     leds[i] = ColorFromPalette(palette, gHue+(i*2), gHue+(i*10));
   }
}

 
