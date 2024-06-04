#include <Adafruit_CircuitPlayground.h>


volatile int SwitchFlag = false;
volatile int ButtonFlag = false;
volatile int SwitchState = false;
int delaytime = 1500;


void setup() {
  CircuitPlayground.begin();
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(7), switchISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(4), buttonISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(5), buttonISR, FALLING);
}

void loop() {
  SwitchState = CircuitPlayground.slideSwitch();
  int randNumber = random(10);
  CircuitPlayground.setPixelColor(randNumber, 0xFFFFFF);
  if(SwitchState == true) {
    for(int led = 0; led < 10; led++)
    {
      SwitchState = CircuitPlayground.slideSwitch();
      CircuitPlayground.setPixelColor(led, 255,   0,   0);
      delay(delaytime);
      CircuitPlayground.setPixelColor(led, 0,   0,   0);
      if (led == randNumber) {
        CircuitPlayground.setPixelColor(randNumber, 0xFFFFFF);
      }
    }
  }
}

void switchISR() {
  SwitchFlag = true;
}

void buttonISR() {
  ButtonFlag = true;
}