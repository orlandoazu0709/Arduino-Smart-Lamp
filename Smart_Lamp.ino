//ANIMATED DINOSAUR PIXEL PANEL / SMART LAMP                                   Created by Orlando Azuara

#include "FastLED.h"

#define NUM_LEDS 50
#define BUTTON 2
#define DATA_PIN 3
#define LIGHT_SENSOR A0

CRGB leds[NUM_LEDS];

void setup() {
  delay(2000);
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(255); //Number 0-255
  FastLED.clear();
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(LIGHT_SENSOR, INPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON), myISR, RISING);
}
int hue = 0;
int buttonState = 1;
volatile int mode = 0;
int prevState = 1;
int prevHue;
int void_pixels[] = {2, 4, 6, 8, 9, 10, 20, 26, 36, 38, 40, 42, 43, 45, 47, 48};
int dino_belly[] = {12, 13, 14, 15, 22, 24, 25};


void loop() {
  if (mode == 0) {
    whiteLight(); // default white light
  }
  else if (mode == 1) {
    colorFade(); // soft color switch as panel
  }
  else if (mode == 2) {
    sensorLight(); // increases/decreases brightness if light sensor is below 400
  }
  else if (mode == 3){
    colorFlicker(); // hard color switch by row
  }
  else if (mode == 4) {
    showDinosaur(); //dinosaur sprite
  }
}

void myISR() {
  mode = (mode + 1) % 5;
}



/****** LED MODES *********/

void whiteLight() {
  FastLED.setBrightness(255);
  fill_solid(leds, NUM_LEDS, CRGB::White);
  FastLED.show();
}

void sensorLight() {
  int sensor = analogRead(LIGHT_SENSOR);
  fill_solid(leds, NUM_LEDS, CRGB::White); 

  int brightness; //= map(sensor, 0, 600, 0, 255);
  if (sensor < 200) {
    brightness = 0;
  }
  else if (sensor < 400) {
    brightness = 128;
  }
  else {
    brightness = 255;
  }
  FastLED.setBrightness(brightness);
  FastLED.show();
  //delay(50);
}

void showDinosaur() {
  FastLED.setBrightness(255);
  //fills the panel with body color
  fill_solid(leds, NUM_LEDS, CRGB::Green);

  //turns off background pixels to create body "frame"
  for (int i=0; i<16; i++) {
    leds[void_pixels[i]] = CRGB::Black;
  }

  //sets dinosaur spine
  for (int i=35; i<40; i+=2){
    leds[i] = CRGB::Orange;
  }

  //sets dinosaur belly
  for (int i=0; i<7; i++) {
    leds[dino_belly[i]] = CRGB::LightGreen;
  }

  FastLED.show();
  delay(500);

  //pixel displacement for dinosaur movement
  for (int i=7; i<12; i+=4){
    leds[i] = CRGB::Black;
    leds[i-1] = CRGB::Green;
  }

  FastLED.show();
  delay(500);
  
  
}

void colorFade() {
  FastLED.setBrightness(255);
  for (int i=0; i<49; i++){
    leds[i] = CHSV(hue, 255, 255);
  }
  FastLED.show();
  hue = (hue + 1) % 256;
  delay(40); 
}

void colorFlicker() {
  FastLED.setBrightness(255);
  for (int i=1; i<49; i++){
    leds[i-1] = CHSV(hue, 255, 255);
    leds[i] = CHSV(hue, 255, 255);
    FastLED.show();
    hue = (hue + 4) % 256;
    delay(40);
  }
}
