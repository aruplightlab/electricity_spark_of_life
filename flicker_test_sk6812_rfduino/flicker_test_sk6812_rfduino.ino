// Flicker simulation for SK6812 tri-white LED strip, using the FastLED library
// This is the LED product used and responds to the WS2812B configuration
// https://www.aliexpress.com/item/SK6812WWA-SK6812-IC-warm-white-cool-white-amber-chips-inside-led-addressable-strip-60leds-m-non/32404277416.html?spm=2114.13010608.0.0.kM2lxC
// Connect the data wire to pin 3 and the ground to the Arduino ground (this is important!)
// This is used for the ELECTRICITY sign for the exhibition at the WELLCOME Collection
// Now including RFduino code for BLE connectivity
// Dec 2016, Inessa Demidova
// Feb 2017, Francesco Anselmo francesco.anselmo@arup.com


#include <SPI.h>
#include <RFduinoBLE.h>

#include "FastLED.h"

// How many leds are in the strip?
#define NUM_LEDS 240

// Data pin that led data will be written out over
#define DATA_PIN 2

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];


// Configurations of the ELECTRICITY letters
int letters_length[] = {
  11, // E
  11, // L
  12, // E
  14, // C
  12, // T
  13, // R
  7, // I
  13, // C
  6, // I
  12, // T
  9, // Y
  9, // Y
  12, // T
  6, // I
  13, // C
  7, // I
  13, // R
  12, // T
  14, // C
  12, // E
  11, // L
  11, // E
};

int letters_start[] = {
  0, // E
  11, // L
  11 + 11, // E
  11 + 11 + 12, // C
  11 + 11 + 12 + 14, // T
  11 + 11 + 12 + 14 + 12, // R
  11 + 11 + 12 + 14 + 12 + 13, // I
  11 + 11 + 12 + 14 + 12 + 13 + 7, // C
  11 + 11 + 12 + 14 + 12 + 13 + 7 + 13, // I
  11 + 11 + 12 + 14 + 12 + 13 + 7 + 13 + 6, // T
  11 + 11 + 12 + 14 + 12 + 13 + 7 + 13 + 6 + 12, // Y
  11 + 11 + 12 + 14 + 12 + 13 + 7 + 13 + 6 + 12 + 9, // Y
  11 + 11 + 12 + 14 + 12 + 13 + 7 + 13 + 6 + 12 + 9 + 9, // T
  11 + 11 + 12 + 14 + 12 + 13 + 7 + 13 + 6 + 12 + 9 + 9 + 12, // I
  11 + 11 + 12 + 14 + 12 + 13 + 7 + 13 + 6 + 12 + 9 + 9 + 12 + 6, // C
  11 + 11 + 12 + 14 + 12 + 13 + 7 + 13 + 6 + 12 + 9 + 9 + 12 + 6 + 13, // I
  11 + 11 + 12 + 14 + 12 + 13 + 7 + 13 + 6 + 12 + 9 + 9 + 12 + 6 + 13 + 7, // R
  11 + 11 + 12 + 14 + 12 + 13 + 7 + 13 + 6 + 12 + 9 + 9 + 12 + 6 + 13 + 7 + 13, // T
  11 + 11 + 12 + 14 + 12 + 13 + 7 + 13 + 6 + 12 + 9 + 9 + 12 + 6 + 13 + 7 + 13 + 12, // C
  11 + 11 + 12 + 14 + 12 + 13 + 7 + 13 + 6 + 12 + 9 + 9 + 12 + 6 + 13 + 7 + 13 + 12 + 14, // E
  11 + 11 + 12 + 14 + 12 + 13 + 7 + 13 + 6 + 12 + 9 + 9 + 12 + 6 + 13 + 7 + 13 + 12 + 14 + 12, // L
  11 + 11 + 12 + 14 + 12 + 13 + 7 + 13 + 6 + 12 + 9 + 9 + 12 + 6 + 13 + 7 + 13 + 12 + 14 + 12 + 11 // E
};

int NUM_LETTERS = sizeof(letters_length) / sizeof(int); // get number of letters from the arra

int lettersValue[] = {
  11, // E
  11, // L
  12, // E
  14, // C
  12, // T
  13, // R
  7, // I
  13, // C
  6, // I
  12, // T
  9, // Y
  9, // Y
  12, // T
  6, // I
  13, // C
  7, // I
  13, // R
  12, // T
  14, // C
  12, // E
  11, // L
  11, // E
};

bool autoMode = false;
int mode = 4;                   // lighting mode
// 0 = off
// 1 = cycle through letters
// 2 = random flicker
// 3 = sine wave all together
// 4 = sine wave random
int ledValue[NUM_LEDS];         // state of each led
int buttonPress;
long randNumber;
unsigned long interval = 1000;
unsigned long previousTime;
int speedValue = 100;
int wait = 500;

int R = 0;
int G = 0;
int B = 0;


void setup() {

  // Enable serial debug.
  Serial.begin(9600);
  Serial.println("Electricity: the spark of life");
  Serial.println("Serial rate set to 9600 baud");

  // Check RFduino CPU temperature, and print to log
  float CPU_temperature = RFduino_temperature(CELSIUS);
  Serial.print("RFduino_temperature is: ");
  Serial.print(CPU_temperature);
  Serial.println(" deg C");

    // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(2000);
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);

  Serial.begin(9600);           // start serial communication


  pinMode(8, INPUT);            // set input for button
  speedValue = analogRead(1);   // read potentiometer
  randomSeed(analogRead(0));    // seed the rnd generator with noise from unused pin
  //randomSeed(millis());    // seed the rnd generator with noise from unused pin
  previousTime = millis();

  for (int i = 0; i <= (NUM_LETTERS - 1); i++) { // init each led with a random value
    lettersValue[i] = random(20, 255);
  }

  Serial.print("Mode:");
  Serial.println(mode);


  // this is the data we want to appear in the advertisement
  // (the deviceName length plus the advertisement length must be <= 18 bytes
  // RFduinoBLE.advertisementData = "whatever";
  RFduinoBLE.advertisementInterval = 500;
  Serial.println("RFduino BLE Advertising interval 500ms");
  RFduinoBLE.deviceName = "Electricity";
  Serial.println("RFduino BLE DeviceName: Electricity");
  RFduinoBLE.txPowerLevel = -20;
  Serial.println("RFduino BLE Tx Power Level: -20dBm");

  // start the BLE stack
  RFduinoBLE.begin();
  Serial.println("RFduino BLE stack started");

}

void loop() {
  

  // switch to lower power mode
  //  RFduino_ULPDelay(INFINITE);

    if (mode == 0) {
      for (int letter = 0; letter < NUM_LETTERS; letter = letter + 1) {
        fill_gradient(leds, letters_start[letter], CHSV(0, 0, 0), letters_start[letter] + letters_length[letter], CHSV(0, 0, 0), SHORTEST_HUES);
        FastLED.show();
      }
    } else if (mode == 1) {
    for (int letter = 0; letter < NUM_LETTERS; letter = letter + 1) {
      fill_gradient(leds, letters_start[letter], CHSV(100, 255, 255), letters_start[letter] + letters_length[letter], CHSV(0, 255, 0), SHORTEST_HUES);
      FastLED.show();
      //      for (int led = letters_start[letter]; led < (letters_start[letter]+letters_length[letter]); led = led + 1) {
      //        leds[led] = CRGB::Green;
      //
      //        // Show the leds (only one of which is set to white, from above)
      //        FastLED.show();
      //      }
      // Wait a little bit
      delay(wait);
      for (int led = letters_start[letter]; led < (letters_start[letter] + letters_length[letter]); led = led + 1) {
        // Turn our current led back to black for the next loop around
        leds[led] = CRGB::Black;
      }
    }
  }
  else if (mode == 2)
  {
    for (int i = 0; i <= (NUM_LETTERS - 1); i++) {           // for each led:
      //              DmxSimple.write(i + 1, ledValue[i]);   // set the pwm value of that pin determined previously
      randNumber = random(-35, 45);            // generate new random number and add that to the current value
      lettersValue[i] += randNumber;               // that range can be tweaked to change the intensity of the flickering
      fill_gradient(leds, letters_start[i], CHSV(160, 255, lettersValue[i]), letters_start[i] + letters_length[i], CHSV(0, 255, lettersValue[i]), SHORTEST_HUES);
      FastLED.show();
      //    if (ledValue[i] > 200) {                 // clamp the limits of the pwm values so it remains within
      //      ledValue[i] = 200;                     // a pleasing range as well as the pwm range
      //   }
      //   if (ledValue[i] < 10) {
      //     ledValue[i] = 10;
      //   }
    }
    //    delay(wait);    // the delay between changes
    //    testButton();
  }
  else if (mode == 3)
  {
    float in, out;
    for (in = 0; in < 6.283; in = in + 0.0001 * speedValue)
    {
      out = sin(in) * 127.5 + 127.5;
//      Serial.println(out);
      for (int i = 0; i <= (NUM_LETTERS - 1); i++) {
        fill_gradient(leds, letters_start[i], CHSV(160, 255, out), letters_start[i] + letters_length[i], CHSV(0, 255, out), SHORTEST_HUES);
        FastLED.show();
        //            DmxSimple.write(i + 1, out);
      }
      //          testButton();
    }
  }
  else if (mode == 4)
  {
    float in, out;
    for (in = 0; in < 6.283; in = in + 1)
    {
      out = sin(in) * 255;
      for (int i = 0; i <= (NUM_LETTERS - 1); i++) {
        fill_gradient(leds, letters_start[i], CHSV(160, 255, lettersValue[i] + out), letters_start[i] + letters_length[i], CHSV(0, 255, lettersValue[i] + out), SHORTEST_HUES);
        FastLED.show();
        //        Serial.println(lettersValue[i] + out);
        //DmxSimple.write(i + 1, ledValue[i] + out);
        //  if(mode == 4){
        //    mode = 0;}
        //        Serial.print(ledValue[i] + out);
        //        Serial.print(" ");
      }
      //testButton();
      //      Serial.println();
    }
  }

  EVERY_N_SECONDS( 5 ) {
    randomSeed(millis());  // refresh seed periodically
  }


  EVERY_N_SECONDS( 20 ) {
    if (autoMode) {
      nextMode();  // change patterns periodically
      Serial.print("Mode:");
      Serial.println(mode);
    }
  }
  
}

void nextMode()
{
  // add one to the current pattern number, and wrap around at the end
  mode = (mode + 1) % 5;
}

void RFduinoBLE_onAdvertisement()
{
  Serial.println("RFduino is doing BLE advertising ...");
  // turn off
  fill_gradient(leds, 0, CHSV(100, 255, 0), NUM_LEDS, CHSV(0, 255, 0), SHORTEST_HUES);
  FastLED.show();
}

void RFduinoBLE_onConnect()
{
  Serial.println("RFduino BLE connection successful");
  fill_gradient(leds, 0, CHSV(0, 255, 100), NUM_LEDS, CHSV(0, 255, 100), SHORTEST_HUES);
  FastLED.show();
  delay(500);
  fill_gradient(leds, 0, CHSV(100, 255, 100), NUM_LEDS, CHSV(100, 255, 100), SHORTEST_HUES);
  FastLED.show();
  delay(500);
  fill_gradient(leds, 0, CHSV(255, 255, 100), NUM_LEDS, CHSV(255, 255, 100), SHORTEST_HUES);
  FastLED.show();
  delay(500);
  fill_gradient(leds, 0, CHSV(0, 0, 100), NUM_LEDS, CHSV(0, 0, 100), SHORTEST_HUES);
  FastLED.show();
  delay(500);
}

void RFduinoBLE_onDisconnect()
{
  Serial.println("RFduino BLE disconnected");
}

void RFduinoBLE_onReceive(char *data, int len)
{
  // if the first byte is 0x01 / on / true
  Serial.println("Received data over BLE");
  Serial.println(len);
  uint8_t r;
  uint8_t g;
  uint8_t b;
  if (len==3) {
    uint8_t r = data[0];
    uint8_t g = data[1];
    uint8_t b = data[2];
    Serial.println(r);
    Serial.println(g);
    Serial.println(b);
    fill_gradient(leds, 0, CHSV(r, g, b), NUM_LEDS, CHSV(r, g, b), SHORTEST_HUES);
    FastLED.show();
    Serial.println("Turn light on");
  } else if (len==1) {
    uint8_t myByte = data[0]; // store first char in array to myByte 
    Serial.println(myByte); // print myByte via serial
    mode = myByte;
    if (myByte==0) {
//      fill_gradient(leds, 0, CHSV(0, 0, 0), NUM_LEDS, CHSV(0, 0, 0), SHORTEST_HUES);
//      FastLED.show();
      Serial.println("Turn light off");  
    }
  }

  /*
  if (data[0])
  {
    colorWipe( lampan.Color(255, 255, 255), 50 );
    Serial.println("Turn light on");
  }
  else
  {
    colorWipe( lampan.Color(0, 0, 0), 50 );
    Serial.println("Turn light off");
  }
  */
}




