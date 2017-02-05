// Flicker simulation for SK6812 tri-white LED strip, using the FastLED library
// This is the LED product used and responds to the WS2812B configuration
// https://www.aliexpress.com/item/SK6812WWA-SK6812-IC-warm-white-cool-white-amber-chips-inside-led-addressable-strip-60leds-m-non/32404277416.html?spm=2114.13010608.0.0.kM2lxC
// Connect the data wire to pin 3 and the ground to the Arduino ground (this is important!)
// This is used for the ELECTRICITY sign for the exhibition at the WELLCOME Collection
// Dec 2016, Inessa Demidova
// Feb 2017, Francesco Anselmo

#include "FastLED.h"

// How many leds are in the strip?
#define NUM_LEDS 120

// Data pin that led data will be written out over
#define DATA_PIN 3

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];

// Configurations of the ELECTRICITY letters
int letters[] = {
  11, // E
  11, // L
  12, // E
  14, // C
  12, // T
  13, // R
   7, // I
  13, // C
   7, // I 
  12, // T
   8  // Y 
};

int letters_start[] = {
  0, // E
  11, // L
  11+11, // E
  11+11+12, // C
  11+11+12+14, // T
  11+11+12+14+12, // R
  11+11+12+14+12+13, // I
  11+11+12+14+12+13+7, // C
  11+11+12+14+12+13+7+13, // I 
  11+11+12+14+12+13+7+13+7, // T
  11+11+12+14+12+13+7+13+7+12  // Y 
};

int NUM_LETTERS = sizeof(letters)/sizeof(int);

int mode = 0;                   // lighting mode 
                                // 0 = cycle through letters
                                // 1 = random flicker
                                // 2 = sine wave all together
                                // 3 = sine wave random
int ledValue[NUM_LEDS];         // state of each led
int buttonPress;
long randNumber;
unsigned long interval = 1000;
unsigned long previousTime;
int speedValue;

void setup() {
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(2000);
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  
  Serial.begin(9600);           // start serial communication

  
  pinMode(8, INPUT);            // set input for button
  speedValue = analogRead(1);   // read potentiometer
  randomSeed(analogRead(0));    // seed the rnd generator with noise from unused pin
  previousTime = millis(); 
//  for (int i = 0; i <= (channelNo - 1); i++) { // init each led with a random value
//    ledValue[i] = random(20, 201);
//  }
}

void loop() {

  if (mode == 0) {
    for(int letter = 0; letter < NUM_LETTERS; letter = letter + 1) {
      for (int led = letters_start[letter]; led < (letters_start[letter]+letters[letter]); led = led + 1) {
        leds[led] = CRGB::Green;
        // Show the leds (only one of which is set to white, from above)
        FastLED.show();
      }
        // Wait a little bit
        delay(500);
      for (int led = letters_start[letter]; led < (letters_start[letter]+letters[letter]); led = led + 1) {
        // Turn our current led back to black for the next loop around
        leds[led] = CRGB::Black;
      }
    }    
  }
  else if (mode == 1)
  {
//    for (int i = 0; i <= (channelNo - 1); i++) {           // for each led:
//      DmxSimple.write(i + 1, ledValue[i]);   // set the pwm value of that pin determined previously
//      randNumber = random(-35, 45);            // generate new random number and add that to the current value
//      ledValue[i] += randNumber;               // that range can be tweaked to change the intensity of the flickering
//      //    if (ledValue[i] > 200) {                 // clamp the limits of the pwm values so it remains within
//      //      ledValue[i] = 200;                     // a pleasing range as well as the pwm range
//      //   }
//      //   if (ledValue[i] < 10) {
//      //     ledValue[i] = 10;
//      //   }
//    }
//    delay(speedValue);    // the delay between changes
//    testButton();
  }
  else if (mode == 2)
  {
//    float in, out;
//    for (in = 0; in < 6.283; in = in + 0.000001*speedValue)
//    {
//      out = sin(in) * 127.5 + 127.5;
//      for (int i = 0; i <= (channelNo - 1); i++) {
//        DmxSimple.write(i + 1, out);
//      }
//      testButton();
//    }
  }
  else if (mode == 3)
  {
//    float in, out;
//    for (in = 0; in < 6.283; in = in + 0.00001*speedValue)
//    {
//      out = sin(in) * 127.5 - 127.5;
//      for (int i = 0; i <= (channelNo - 1); i++) {
//        DmxSimple.write(i + 1, ledValue[i] + out);
//        //  if(mode == 4){
//        //    mode = 0;}
//        //        Serial.print(ledValue[i] + out);
//        //        Serial.print(" ");
//      }
//      testButton();
//      //      Serial.println();
//    }
   }

}


void testButton ()
{
  if ((millis() - previousTime) > interval) {
    buttonPress = digitalRead(8); //read button state
    speedValue = analogRead(1); // read potentiometer
    Serial.print(mode);
    Serial.print(" ");
    Serial.print(speedValue*.000001);
    Serial.print(" ");
    Serial.print(speedValue*.00001);
    Serial.print(" ");
    Serial.println(speedValue);
    if (buttonPress == HIGH) {
      //    delay(500);
      mode ++;
      //Reset count if over max mode number
      if (mode == 4)
      {
        mode = 1;
      }
    }
    previousTime = millis();   
  }
  
}


