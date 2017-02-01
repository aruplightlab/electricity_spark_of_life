//Flicker simulation for Tinker.it DMX shield controlling 12No 24V LED strips via EldoLED DMX driver
//Dec 2016, Inessa Demidova
#include <DmxSimple.h>
#define channelNo 12

int mode = 1;                            //dimming mode- 1=random flicker, 2=sine wave all together, 3=sine wave random
int ledValue[channelNo];                 // last state of each led
int buttonPress;
long randNumber;
unsigned long interval = 1000;
unsigned long previousTime;
int speedValue;

void setup() {
  /* The most common pin for DMX output is pin 3, which DmxSimple
  ** uses by default. If you need to change that, do it here. */
  DmxSimple.usePin(3);
  Serial.begin(9600);

  /* DMX devices typically need to receive a complete set of channels
  ** even if you only need to adjust the first channel. You can
  ** easily change the number of channels sent here. If you don't
  ** do this, DmxSimple will set the maximum channel number to the
  ** highest channel you DmxSimple.write() to. */
  DmxSimple.maxChannel(sizeof(ledValue));
  pinMode(8, INPUT);
  speedValue = analogRead(1); // read potentiometer
  randomSeed(analogRead(0));     // seed the rnd generator with noise from unused pin
  previousTime = millis(); 
  for (int i = 0; i <= (channelNo - 1); i++) { // init each led with a random value
    ledValue[i] = random(20, 201);
  }
}

void loop() {

  if (mode == 1)
  {
    for (int i = 0; i <= (channelNo - 1); i++) {           // for each led:
      DmxSimple.write(i + 1, ledValue[i]);   // set the pwm value of that pin determined previously
      randNumber = random(-35, 45);            // generate new random number and add that to the current value
      ledValue[i] += randNumber;               // that range can be tweaked to change the intensity of the flickering
      //    if (ledValue[i] > 200) {                 // clamp the limits of the pwm values so it remains within
      //      ledValue[i] = 200;                     // a pleasing range as well as the pwm range
      //   }
      //   if (ledValue[i] < 10) {
      //     ledValue[i] = 10;
      //   }
    }
    delay(speedValue);    // the delay between changes
    testButton();
  }
  else if (mode == 2)
  {

    float in, out;
    for (in = 0; in < 6.283; in = in + 0.000001*speedValue)
    {
      out = sin(in) * 127.5 + 127.5;
      for (int i = 0; i <= (channelNo - 1); i++) {
        DmxSimple.write(i + 1, out);
      }
      testButton();
    }
  }
  else if (mode == 3)
  {
    float in, out;
    for (in = 0; in < 6.283; in = in + 0.00001*speedValue)
    {
      out = sin(in) * 127.5 - 127.5;
      for (int i = 0; i <= (channelNo - 1); i++) {
        DmxSimple.write(i + 1, ledValue[i] + out);
        //  if(mode == 4){
        //    mode = 0;}
        //        Serial.print(ledValue[i] + out);
        //        Serial.print(" ");
      }
      testButton();
      //      Serial.println();
    }
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


