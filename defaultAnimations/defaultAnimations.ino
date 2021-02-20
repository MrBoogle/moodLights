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

SimplePatternList gPatterns = {defaultMode, lightning, rain, snow, clearMode, cloudy, ambient};

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

bool notif = false;
  
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
  Serial.println(analogRead(A0));
  Serial.println(analogRead(A1));
  Serial.println(analogRead(A2));
  Serial.println(analogRead(A3));
  Serial.println("---");
  //Could use map for better code but arduino barely has any memory
  // > 600 means pin is high

  //defaultMode
  if (analogRead(A0) < 600 && analogRead(A1) < 600 && analogRead(A2) < 600 && analogRead(A3) < 600) {
    gCurrentPatternNumber = 4;
    } 
  //lightning
  if (analogRead(A0) < 600 && analogRead(A1) < 600 && analogRead(A2) < 600 && analogRead(A3) > 600) {
    gCurrentPatternNumber = 1;
    } 
  if (analogRead(A0) < 600 && analogRead(A1) < 600 && analogRead(A2) > 600 && analogRead(A3) < 600) {
    gCurrentPatternNumber = 2;
    } 
  if (analogRead(A0) < 600 && analogRead(A1) < 600 && analogRead(A2) > 600 && analogRead(A3) > 600) {
    gCurrentPatternNumber = 3;
    } 
  if (analogRead(A0) < 600 && analogRead(A1) > 600 && analogRead(A2) < 600 && analogRead(A3) < 600) {
    gCurrentPatternNumber = 4;
    } 
  if (analogRead(A0) < 600 && analogRead(A1) > 600 && analogRead(A2) < 600 && analogRead(A3) > 600) {
    gCurrentPatternNumber = 5;
    } 
  if (analogRead(A4) > 600) {
    notif = true;
    } else {
      notif = false;
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
 // sinelon();
  bpm();
  //juggle();
  }

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}


void clouds() {
  for (int i = 109; i < NUM_LEDS; i++) {
    leds[i] = CRGB::DarkGray;
    }

    //Random, 95 - 109
    for (int i = 0; i < 5; i++) {
    int randPos = rand() % 13 + 95;
    leds[randPos] += CRGB(128, 128, 128);
    }
    

}

void rain()
{
    
    for (int i = 0; i < 110; i++) {
    leds[i] = CRGB::Black;
    }

clouds();
    
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
      leds[96] = leds[80] = leds[64] = leds[49] = leds[34] = leds[18] = leds[2] = CRGB::Yellow;
      leds[100] = leds[84] = leds[68] = leds[53] = leds[38] = leds[22] = leds[6] = CRGB::Yellow;
      leds[100+4] = leds[84+4] = leds[68+4] = leds[53+4] = leds[38+4] = leds[22+4] = leds[6+4] = CRGB::Yellow;
      leds[100+8] = leds[84+8] = leds[68+8] = leds[53+8] = leds[38+8] = leds[22+8] = leds[6+8] = CRGB::Yellow;
      FastLED.delay(150);
      leds[96] = leds[80] = leds[64] = leds[48] = leds[34] = leds[18] = leds[2] = CRGB::Black;
      leds[100] = leds[84] = leds[68] = leds[53] = leds[38] = leds[22] = leds[6] = CRGB::Black;
      leds[100+4] = leds[84+4] = leds[68+4] = leds[53+4] = leds[38+4] = leds[22+4] = leds[6+4] = CRGB::Black;
      leds[100+8] = leds[84+8] = leds[68+8] = leds[53+8] = leds[38+8] = leds[22+8] = leds[6+8] = CRGB::Black;
      FastLED.delay(50);
      leds[96] = leds[80] = leds[64] = leds[49] = leds[34] = leds[18] = leds[2] = CRGB::Yellow;
      leds[100] = leds[84] = leds[68] = leds[53] = leds[38] = leds[22] = leds[6] = CRGB::Yellow;
      leds[100+4] = leds[84+4] = leds[68+4] = leds[53+4] = leds[38+4] = leds[22+4] = leds[6+4] = CRGB::Yellow;
      leds[100+8] = leds[84+8] = leds[68+8] = leds[53+8] = leds[38+8] = leds[22+8] = leds[6+8] = CRGB::Yellow;
      FastLED.delay(150);
      leds[96] = leds[80] = leds[64] = leds[48] = leds[34] = leds[18] = leds[2] = CRGB::Black;
      leds[100] = leds[84] = leds[68] = leds[53] = leds[38] = leds[22] = leds[6] = CRGB::Black;
      leds[100+4] = leds[84+4] = leds[68+4] = leds[53+4] = leds[38+4] = leds[22+4] = leds[6+4] = CRGB::Black;
      leds[100+8] = leds[84+8] = leds[68+8] = leds[53+8] = leds[38+8] = leds[22+8] = leds[6+8] = CRGB::Black;
      
      

  }


void snow () {
   for (int i = 0; i < 110; i++) {
    leds[i] = CRGB::DarkBlue;
    }

clouds();
    
    //96, 80, 64, 48, 32, 16, 0
  
    for (int i = 96; i >= 0; i-=16) { 
    leds[i+2] = leds[i+4] = leds[i+6] = leds[i+8] = leds[i+12] = leds[i+10] = leds[i+14] = leds[i] = CRGB::Gray;
    //leds[i+2] = leds[i+4] = leds[i+6] = leds[i+8]
    FastLED.delay(200);
    if (i != 0) {
    leds[i+2-16] = leds[i+4-16] = leds[i+6-16] = leds[i+8-16] = leds[i-16] = leds[i+12-16] = leds[i+10-16] = leds[i+14-16] = CRGB::Gray;}
    leds[i+2] = leds[i+4] = leds[i+6] = leds[i+8] = leds[i+12] = leds[i+10] = leds[i+14]= leds[i] = CRGB::DarkBlue;
    FastLED.delay(200);
  
}
}


/*
 * Cloudy and Clear Section
 */

void blue_skies () {
   //Set all leds to blue
   for (int i= 0; i < NUM_LEDS; i++) {
     leds[i] = CRGB(0, 0, 255);
   }
}

void addSun () {
  for (int j = 0; j < 16; j+= 4) {
  for (int i = 128+j; i < 132+j; i++) {
  leds[i] = CRGB::Yellow;
  }
  for (int i = 113+j; i < 115+j; i++) {
  leds[i] = CRGB::Yellow;
  }
  leds[98+j] = CRGB::Yellow;
  leds[127] = CRGB::Yellow;
  }
}

void clearMode () {
  blue_skies();
  addSun();
}




void cloudy() {

  for (int i= 0; i < NUM_LEDS; i++) {
     leds[i] = CRGB::DarkBlue;
   }
  
  clouds();
  FastLED.delay(2000);
  
  
}

void notification () {
  leds[124] = CRGB(abs(255-leds[124].r), abs(255-leds[124].g), (255-leds[124].b));
  leds[120] = CRGB(abs(255-leds[120].r), abs(255-leds[120].g), (255-leds[120].b));
  leds[116] = CRGB(abs(255-leds[116].r), abs(255-leds[116].g), (255-leds[116].b));
  leds[112] = CRGB(abs(255-leds[112].r), abs(255-leds[112].g), (255-leds[112].b));
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
  if (notif) {
    notification();
    }
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

 
