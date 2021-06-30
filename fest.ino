#include <bitswap.h>
#include <chipsets.h>
#include <color.h>
#include <colorpalettes.h>
#include <colorutils.h>
#include <controller.h>
#include <cpp_compat.h>
#include <dmx.h>
#include <FastLED.h>
#include <fastled_config.h>
#include <fastled_delay.h>
#include <fastled_progmem.h>
#include <fastpin.h>
#include <fastspi.h>
#include <fastspi_bitbang.h>
#include <fastspi_dma.h>
#include <fastspi_nop.h>
#include <fastspi_ref.h>
#include <fastspi_types.h>
#include <hsv2rgb.h>
#include <led_sysdefs.h>
#include <lib8tion.h>
#include <noise.h>
#include <pixelset.h>
#include <pixeltypes.h>
#include <platforms.h>
#include <power_mgt.h>

#include <FastLED.h>
#define LED_PIN     8
#define DATA_PIN    8
#define NUM_LEDS    300
#define COLOR_ORDER GRB
#define LED_TYPE    WS2812

CRGB leds[NUM_LEDS];
int randomR;
int randomG;
int randomB;

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
}

void loop() {
  int brightness = 150;    
  int timer = 30;

  for (int i = 0; i<500; i++){
      // random colored speckles that blink in and fade smoothly
    fadeToBlackBy( leds, NUM_LEDS, 10);
    int pos = random16(NUM_LEDS);
    leds[pos] += CHSV( i + random8(64), 200, 255);
    FastLED.show();
  }  
  
  // length of stripe, brightness, delay
  striped(25, brightness, 1);
  // BEHOLD
  
  // Tail kalles med lengde på tail. og step. samt brightness.
  tail(50, 1, brightness);
  // BEHOLD

  // Fargen vi skal ha og styrke på fargen.
  on_one_by_one("red", brightness);
  // Fra red, green eller blue, til en annen farge. (Kan ikke være fra og til samme.)
  fade_from(brightness, timer, "red", "green");
  // BEHOLD

  for (int i = 0; i<500; i++){
    // eight colored dots, weaving in and out of sync with each other
    fadeToBlackBy( leds, NUM_LEDS, 20);
    byte dothue = 0;
    for( int i = 0; i < 8; i++) {
      leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
      dothue += 32;
    }
    FastLED.show();
    delay(1);
  }
  // BEHOLD
  
  // Mengde med anchors, og størrelsen på anchoret.
  anchor(8, 50, brightness);
  // BEHOLD
  
  portal(2, brightness);
  anchor(8, 50, brightness);
  on_one_by_one("blue", brightness);
  fade_from(brightness, timer, "blue", "red");
  for (int i = 0; i<500; i++){
    // a colored dot sweeping back and forth, with fading trails
    fadeToBlackBy( leds, NUM_LEDS, 20);
    int pos = beatsin16( 13, 0, NUM_LEDS-1 );
    leds[pos] += CHSV( i, 255, 192);
    FastLED.show();
    delay(1);
  }
  tail(80, 3, brightness);
  anchor(8, 50, brightness);
}

// Length of stripe, brightness, speed.
void striped(int len, int brightness, int delayer){
  int r = random(brightness);
  int g = brightness-r;
  for (int offset = 0; offset < len*2; offset++){
    bool s = false;
    for (int i = 0; i < NUM_LEDS; i++){ 
      
      // Switch for å bestemme farge vi lyser opp.     
      if (i % len == 0){
        if (s == false){
          s = true;
        } else if (s == true){
          s = false;   
        }
      }  

      // Endrer slik at vi alltid er innen led stripen.
      int c_light = i + offset;
      if (c_light > NUM_LEDS){
          c_light = c_light - NUM_LEDS;
      }
  
      // Lyser opp lysene i riktig farge og sted.
      if (s == false){
        leds[c_light] = CRGB(r, g, 0);      
      } else {
        leds[c_light] = CRGB(0, 0, brightness);  
      }
    }
    FastLED.show();  
    delay(delayer);
  }
  
}

// Amount = mengde med sparkles for hvert kall.
// Width = hvor mange piksler som lyser opp.
void sparkle(int amount, int width, int r, int  g, int b){
  int led;
  for (int i = 0; i < amount; i++){
    led = random(NUM_LEDS);
    leds[led] = CRGB(r, g, b);
    // Skrur på i bredden
    for (int offset = 0; offset < width; offset++){
      if (led + offset < NUM_LEDS){
        leds[led+offset] = CRGB(r, g, b);  
      }
    }
  }
  FastLED.show();
  // Skrur lysene av igjen.
  fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0));
  FastLED.show();  
}


void fade_from(int brightness, int d, String from, String to){
  if (brightness > 256){
    brightness = 255;  
  }
  int r = 0;
  int g = 0;
  int b = 0;
  for(int colorStep=0; colorStep<brightness; colorStep++ ){
      if (from == "red"){             // From red
        r = brightness - colorStep;   // Redness starts at full brightness and goes to zero
        if (to == "blue"){
          b = colorStep;              // Blue starts at 0 and goes to full.
          g = 0;                      // No green needed to go from red to blue
        } else if (to == "green"){
          b = 0;
          g = colorStep;
        }
      } else if (from == "blue"){      // From blue
        b = brightness-colorStep;        
        if (to == "red"){
          r = colorStep;          
          g = 0;                  
        } else if (to == "green"){
          r = 0;
          g = colorStep;
        }
      } else if (from == "green"){      // From green
        g = brightness-colorStep;        
        if (to == "red"){
          r = colorStep;          
          b = 0;                  
        } else if (to == "blue"){
          b = colorStep;
          r = 0;
        }
      }
    
      for(int x = 0; x < NUM_LEDS; x++){
          leds[x] = CRGB(r,g,b);
      }
      FastLED.show();
      delay(d); 
  }
}

// Funksjonen blinker lyset på også av en gang. 
// R, G, B, antall ganger lyset blinker på, tid lyset er på, tid lyset er av.
void strobe(int red, int green, int blue, int StrobeCount, int FlashDelay, int EndPause){
  for(int j = 0; j < StrobeCount; j++){
    fill_solid(leds, NUM_LEDS, CRGB(red,green,blue));
    FastLED.show();
    delay(FlashDelay);
    fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0));
    FastLED.show();
  }
 delay(EndPause);
}

// Mengden med anchors, hvor mye hvert anchor brer seg ut.
void anchor(int anchor_amount, int anchor_size, int brightness){
  for (int i = 0; i<anchor_amount; i++){
    // Setter et anker. 
    int anchor = random(0, 300);
    // Deretter går vi ut fra ankeret med random farger
    randomR = random(brightness);
    randomG = random(brightness-randomR);
    randomB = random(brightness-randomR-randomG);
    
    leds[anchor] = CRGB(randomR, randomG, randomB);
    for (int nearest = 0; nearest < anchor_size; nearest++){
      if (anchor-nearest/2 >= 0){
        leds[anchor-nearest/2] = CRGB(randomR, randomG, randomB);  
      }
      if (anchor+nearest/2 <= 300){
        leds[anchor+nearest/2] = CRGB(randomR, randomG, randomB);  
      }
      FastLED.show();
      delay(1);
    }  
  }
}

// Parameter STEP bestemmer hvor mange pixler som lyser opp om gangen i loopene.
// Burde aldri være mindre enn 1. jo høyere jo raskere, 10 er dritfort.
void portal(int steps, int brightness){
  randomR = random(brightness);
  randomG = random(brightness-randomR);
  randomB = random(brightness-randomR-randomG);
    
  // Lyser opp
  for(int i = 0; i < NUM_LEDS/2; i++){
    leds[i] = CRGB(randomR, randomG, randomB);
    leds[NUM_LEDS-i] = CRGB(randomR, randomG, randomB);
    if (i % steps == 0){
      delay(1);
      FastLED.show(); 
    }
  }
  // Fjerner lysene
  for(int i = 0; i < NUM_LEDS/2; i++){
    leds[i] = CRGB(0, 0, 0);
    leds[NUM_LEDS-i] = CRGB(0, 0, 0);
    if (i % steps == 0){
      delay(1);
      FastLED.show(); 
    } 
  }

  randomR = random(brightness);
  randomG = random(brightness-randomR);
  randomB = random(brightness-randomR-randomG);
    
  // Deretter går den baklengs
  for(int i = NUM_LEDS/2; i > 0; i--){
    leds[i] = CRGB(randomR, randomG, randomB);
    leds[NUM_LEDS-i] = CRGB(randomR, randomG, randomB);
    if (i % steps == 0){
      delay(1);
      FastLED.show(); 
    } 
  }
   //Fjerner lysene baklengs og
  for(int i = NUM_LEDS/2; i > 0; i--){
    leds[i] = CRGB(0, 0, 0);
    leds[NUM_LEDS-i] = CRGB(0, 0, 0);
    if (i % steps == 0){
      delay(1);
      FastLED.show(); 
    } 
  }  
  
}

// Param er lengden på tail. S er steps før delay.
void tail(int tail, int s, int brightness){
  randomR = random(brightness);
  randomG = random(brightness-randomR);
  randomB = random(brightness-randomR-randomG);
  // Må bare starte de første tail pixlene.
  for (int x = 0; x < tail; x++){
    leds[x] = CRGB(randomR, randomG, randomB);
    FastLED.show();
    if (x % s == 0){
      delay(1);   
    }
  }
  // Deretter vanlig loop
  for (int head = 0; head < NUM_LEDS; head++){
    for (int tail_section = 0; tail_section < tail; tail_section++){
      if (head + tail_section <= 300){
        leds[head + tail_section] = CRGB(randomR, randomG, randomB);
      }
    }
    leds[head] = CRGB(0, 0, 0);
    FastLED.show();
    if (head % s == 0){
      delay(1);   
    }
  }  
}

// Skrur av lysene fra venstre til høyre.
void off_one_by_one(){
  for (int x = 0; x < NUM_LEDS/2; x++){
    leds[x] = CRGB(0, 0, 0);
    leds[NUM_LEDS/2-x] = CRGB(0, 0, 0);
    delay(1);
    FastLED.show();
  }  
}

// Skrur på lysene med farge C (red, blue green) og brightness. 
void on_one_by_one(String c, int brightness){
  for (int x = 0; x < NUM_LEDS; x++){
    if (c == "red"){
      leds[x] = CRGB(brightness, 0, 0);
      FastLED.show();
      delay(1);
    }
    if (c == "green"){
      leds[x] = CRGB(0, brightness, 0);
      FastLED.show();
      delay(1);
    }
    if (c == "blue"){
      leds[x] = CRGB(0, 0, brightness);
      FastLED.show();
      delay(1);
    }
  }  
}
