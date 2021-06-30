//we have to include the library 
#include "LedControl.h"
//include this file so we can write down a byte in binary encoding
#include <binary.h>
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin
int buttonState = 0;         // variable for reading the pushbutton status
const int width = 4;
const int height = 4; 
int screen_id = 3;
int screen_h = 0; 
int haste = 200;

//create the LedControl 
int DIN = 10;
int CLK = 8;
int CS = 9;
int numOfDisplays = 4;
LedControl lc=LedControl(DIN, CLK, CS, numOfDisplays); 

void setup() {
   pinMode(buttonPin, INPUT);
   Serial.begin(9600);
   //wake up the MAX72XX from power-saving mode 
   lc.shutdown(0,false);
   lc.shutdown(1,false);
   lc.shutdown(2,false);
   lc.shutdown(3,false);
   //set a medium brightness for the Leds. (Goes from 0-15)
   lc.setIntensity(0, 8);
   lc.setIntensity(1, 8);
   lc.setIntensity(2, 8);
   lc.setIntensity(3, 8);
}

void loop() {

  // button high er når den er nede  
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH){
    update_pos();
  }
  
  for (int offset = 0; offset < 5; offset++){
    draw(screen_id, screen_h, offset);  
    delay(haste/2);
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH ){
      update_pos();
    }
    delay(haste/2);
    blank(0, screen_id);
  }
  buttonState = digitalRead(buttonPin);

  for (int offset = 3; offset > 0; offset--){
    draw(screen_id, screen_h, offset);  
    delay(haste/2);
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH and screen_h == 0){
      update_pos();
    }
    delay(haste/2);
    blank(0, screen_id);
  }
  
}

void update_pos(){
  if (screen_h == 0){
    screen_h = 4;
  } else {
    screen_h = 0;
    screen_id --;  
  }
}

void blank(int d, int adr){
  for (int col = 0; col < 8; col++){
    for (int row = 0; row < 8; row++){
    lc.setLed(adr, row, col, false);
    delay(d);
    }  
  }  
}

void draw(int screen_id, int screen_h, int offset){
  for (int w = 0; w < width; w++){
    for (int h = 0; h < height; h++){
      lc.setLed(screen_id, w+4 - screen_h, h + offset, true);
    }  
  }  
}
