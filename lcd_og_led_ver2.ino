// LCD VARIABLENE
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
int xpos = 0;
int ypos = 0;
int vis = 0;
int row = 0;
int settingsDone = 0;

// NEOPIXEL VARIABLENE
#include <Adafruit_NeoPixel.h>
const byte neoPin = 9; // Pinnen vi bruker
const byte neoPixels = 24; // Piksler/leds i ringen
byte neoBright = 100; // Variabel for styrke
int d =  20;
Adafruit_NeoPixel ring = Adafruit_NeoPixel(neoPixels, neoPin, NEO_GRB);

// For random og to_dots funksjonene.
int randomR = 0;
int randomG = 0;
int randomB = 0;
int randomR2 = 0;
int randomG2 = 0;
int randomB2 = 0;
// For to_dots:
int i2pos = 0;

void setup() {
  // Setter opp LCD med 16 kolonner og 2 rader.
  lcd.begin(16, 2);
  Serial.begin(9600);
  // Setter opp ringen
  ring.begin();
  ring.setBrightness(neoBright); 
  ring.show();
}

void loop(){

  lcd.home();
  lcd.print("light:");
  lcd.print(neoBright);
  
  lcd.setCursor(0, 1);
  lcd.print("delay:");
  lcd.print(d);
  lcd.home();

  // x er knapp verdien.
  int x = analogRead(0);   
  int chooser = random(1010, 100);

  if (chooser > 100){
    portal(d);  
   }
  if (chooser == 0){
    //random_colors(d);
    //to_spinnende_dots(d);
  } else if (chooser == 1){
    //random_colors(d);
  } else if (chooser == 2){
    portal(d);  
  } else if (chooser == 3){
    portal(d);
  } else if (chooser == 4){
    //hyperspeed();  
  } else if (chooser == 5){
    //waves(d);  
  } else if (chooser == 6){
    //smileyface(d);
  }
  
  x = analogRead(0);    
}


void settings(){
  Serial.println("Inn i instillinger.");
  // Innstillinger kjører helt til SELECT trykkes.
  int x = analogRead(0);
  while (x > 800 or x < 600){   
    delay(100);
    x = analogRead(0);
        
    // Blinker av og på den rowen man er på.
    if (row == 0 and vis == 1){
      lcd.setCursor(6,0);
      lcd.print(neoBright);
      vis = 0;
    } else if (row == 0 and vis == 0) {
      lcd.setCursor(6,0);
      lcd.print("   ");  
      vis = 1;
    } else if (row == 1 and vis == 1){
      lcd.setCursor(6, 1);
      lcd.print(d);
      vis = 0;
    } else if (row == 1 and vis == 0){
      lcd.setCursor(6, 1);
      lcd.print("   ");
      vis = 1;  
    }

    if (x >= 0 and x < 60){      // RIGHT
      if (row == 0){
        increaseLight();
        lcd.clear();
        lcd.home();
        lcd.print("light:");
        lcd.print(neoBright);
      } else if (row == 1){
        increaseDelay();
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("delay:");
        lcd.print(d);
        lcd.home();
      }
    }
    if (x > 400 and x < 600){   // LEFT
      if (row == 0){
        decreaseLight();
        lcd.clear();
        lcd.home();
        lcd.print("light:");
        lcd.print(neoBright);
      } else if (row == 1){
        decreaseDelay();
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("delay:");
        lcd.print(d);
        lcd.home();
      }
    }  

    // Bytter på hvilken rad man endrer på i innstillinger. 
    // Den raden man går bort fra setter man og til å ha stabil tekst.
    if ((x > 0 and x < 100) or (x > 200 and x < 400) ){ // Up and Down
      if (row == 1){
        lcd.setCursor(6, 1);
        lcd.print(d);
        row = 0;
      } else if (row == 0){
        lcd.setCursor(6, 0);
        lcd.print(neoBright);
        row = 1;
      }
    }
  }
  Serial.println("Ut av innstillinger.");
  lcd.setCursor(6, 0);
  lcd.print(neoBright);
  lcd.setCursor(6, 1);
  lcd.print(d);
  lcd.home();  
  settingsDone = 1;
  ring.setBrightness(neoBright);
  ring.show();
}


void smileyface(int d){
  // Setter smilet.
  randomR = random(0, 255);
  randomG = random(0, 255);
  randomB = random(0, 255);
  
  for (int i = 0; i < neoPixels/2; i++){
    ring.setPixelColor(i, ring.Color(randomR, randomG, randomB));
    ring.show();
    delay(d);
  }
  // Setter øyne.
  ring.setPixelColor(15, ring.Color(randomR, randomG, randomB));
  ring.setPixelColor(20, ring.Color(randomR, randomG, randomB));
  ring.show();
  delay(d*5);
}


void increaseLight(){
  if (neoBright < 100){
    neoBright = neoBright + 10; 
  }
}

void decreaseLight(){
  if (neoBright > 10){
    neoBright = neoBright - 10;  
  }  
}   

void increaseDelay(){
  if (d < 100){
    d = d + 10;  
  }
}

void decreaseDelay(){
  if (d > 10){
    d = d - 10;
  }  
}

void move_o(){
  lcd.clear();
  int x = analogRead(0);
  lcd.setCursor(xpos, ypos);
  lcd.print("O");
  delay(100);

  // Right
  if (x == 0){
    xpos ++;
    return;
  }
  // Left 
  if (x > 400 and x < 600){
    xpos --;
    return;
  }
  // Up
  if (x > 0 and x < 100){
    ypos--;
    return;
  }
  // Down
  if (x > 200 and x < 400){
    ypos++;
    return;
  }  
}

void robotface(){
  lcd.setCursor(5,0);
  lcd.print("|O   O|");
  lcd.setCursor(7,1);
  lcd.print("---");
  delay(1000);
  lcd.setCursor(5,0);
  lcd.print("|-   -|");
  lcd.setCursor(7,1);
  lcd.print("---");
  delay(1000);
  lcd.setCursor(5,0);
  lcd.setCursor(7,1);
  lcd.print("---");
  delay(1000);
  lcd.setCursor(5,0);
  lcd.print("|-   -|");
  lcd.setCursor(7,1);
  lcd.print("---");
  delay(1000);
  lcd.setCursor(5,0);
  lcd.print("|O   O|");
  lcd.setCursor(7,1);
  lcd.print("---");
  delay(4000);  
}


void waves(int delayer){
  randomR = random(255);
  randomG = random(255);
  randomB = random(255);
  for (int i = 0; i < neoPixels/2+1; i++){
    ring.setPixelColor(i, ring.Color(randomR, randomG, randomB));
    ring.setPixelColor(i-2, ring.Color(0, 0, 0));
    // Andre siden:
    ring.setPixelColor(neoPixels-i, ring.Color(randomR, randomG, randomB));
    ring.setPixelColor(neoPixels-i+2, ring.Color(0, 0, 0));
    ring.show();
    delay(delayer);
  }
  blank_out();
}


void hyperspeed(){
  randomR = random(255);
  randomG = random(255);
  randomB = random(255);
  // Akselererer opp en enkelt dott.
  for (int x = 0; x < 20; x++){
    for (int i = 0; i < neoPixels; i++){
      ring.setPixelColor(i, ring.Color(randomR, randomG, randomB));
      ring.setPixelColor(i-1, ring.Color(0,0,0));
        if (i == 0){
          ring.setPixelColor(neoPixels-1, ring.Color(0,0,0));
        }
        ring.show();
        delay(20-x);
    }
  }
  // Etter akselerasjon blir dotten stabil
  for (int i = 0; i < neoPixels; i++){
      ring.setPixelColor(i, ring.Color(randomR, randomG, randomB));
      delay(3);
      ring.show();
  }
  delay(1000);
  
}


void blink(int times){
  // Flasher et random lys "times" antall ganger
  randomR = random(255);
  randomG = random(255);
  randomB = random(255);
  for (int x = 0; x < times; x++) {
    for (int i = 0; i < neoPixels; i++){
      ring.setPixelColor(i, ring.Color(randomR, randomG, randomG));
    }
    ring.show();
    delay(400);
    blank_out();
    delay(200);
  }
}

void portal(int delayer){
  randomR = random(255);
  randomG = random(255);
  randomB = random(255);
  // Lyser opp
  for(int i = 0; i < 1+neoPixels/2; i++){
    ring.setPixelColor(i, ring.Color(randomR, randomG, randomB)); 
    ring.setPixelColor(neoPixels-i, ring.Color(randomR, randomG, randomB)); 
    delay(delayer);
    ring.show(); 
  }
  // Fjerner lysene
  for(int i = 0; i < 1+neoPixels/2; i++){
    ring.setPixelColor(i, ring.Color(0, 0, 0)); 
    ring.setPixelColor(neoPixels-i, ring.Color(0, 0, 0)); 
    delay(delayer);
    ring.show();
  }  
  
  // Deretter går den baklengs
  for(int i = 1+neoPixels/2; i > 0; i--){
    ring.setPixelColor(i-1, ring.Color(randomR, randomG, randomB)); 
    ring.setPixelColor(neoPixels-i, ring.Color(randomR, randomG, randomB)); 
    delay(delayer);
    ring.show(); 
  }
   //Fjerner lysene baklengs og
  for(int i = (neoPixels/2)+1; i > 0; i--){
    ring.setPixelColor(i, ring.Color(0, 0, 0)); 
    ring.setPixelColor(neoPixels-i, ring.Color(0, 0, 0)); 
   // delay(delayer);
    ring.show();
  }  
  
}

// Fjerner alle lysene. Ingen delay.
void blank_out(){
  for(int i = 0; i < neoPixels; i++){
    ring.setPixelColor(i, ring.Color(0,0,0));
  }
  ring.show();
}

// Skrur av lysene en av og. 
void off_one_by_one(int delayer){
  for(int i = 0; i < neoPixels; i++){
    ring.setPixelColor(i, ring.Color(0,0,0));
    ring.show();
    delay(delayer);
  }  
}

// Skrur på randome farger en om gangen
void random_colors(int delayer){
  for(int i = 0; i < neoPixels; i++){
    randomR = random(255);
    randomG = random(255);
    randomB = random(255);
    ring.setPixelColor(i, ring.Color(randomR,randomG,randomB));
    ring.show();
    delay(delayer);
  }
}

// To dots som går om hverandre.
void to_spinnende_dots(int delayer){
  randomR = random(255);
  randomG = random(255);
  randomB = random(255);
  randomR2 = random(255);
  randomG2 = random(255);
  randomB2 = random(255);
  
  for(int i = 0; i < neoPixels; i++){ 
      
    // Første dotten
    ring.setPixelColor(i, ring.Color(randomR,randomG,randomB));
    ring.setPixelColor(i-1, ring.Color(0,0,0));  
    if (i == 0){
      ring.setPixelColor(neoPixels-1, ring.Color(0,0,0));
    }

    // Andre dotten   
    i2pos = i + neoPixels/2;
    // Om vi er på siste pixel må vi endre hvor vi er i forhold til "i" og oppdatere fargen. 
    if (i2pos >= neoPixels) {
      i2pos = i - neoPixels/2;
    }
    ring.setPixelColor(i2pos, ring.Color(randomR2,randomG2,randomB2));
    ring.setPixelColor(i2pos-1, ring.Color(0,0,0));  
    if (i2pos == 0){
      ring.setPixelColor(neoPixels-1, ring.Color(0,0,0));
    }
    ring.show();
    delay(delayer);
  }
  blank_out();
}
