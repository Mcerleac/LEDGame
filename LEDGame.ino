#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>

AsyncDelay delay_1s;
volatile bool SwitchFlag = false;
volatile bool rightButtonFlag = false;
volatile bool leftButtonFlag = false;
bool whileFlag = false;
bool switchState = HIGH;
int delaytime;
int score = 0;
int led;
long randLED;
int switchPin = 7;
int rightButtonPin = 4;
int leftButtonPin = 5;



void setup() {
  CircuitPlayground.begin();
  Serial.begin(9600);
  randomSeed(analogRead(0));
  attachInterrupt(digitalPinToInterrupt(7), switchISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(4), rightbuttonISR, RISING);
  attachInterrupt(digitalPinToInterrupt(5), leftbuttonISR, RISING);
  switchState = digitalRead(switchPin);
  delay_1s.start(1000, AsyncDelay::MILLIS);
  if (!switchState) {
    delaytime = 1000;
  }
  delay(500);
  CircuitPlayground.clearPixels();
  delay(500);
  randLED = random(0, 10);
  CircuitPlayground.setPixelColor(randLED, 255, 255, 255);
  delay(500);
  CircuitPlayground.clearPixels();
}

void loop() {
  if (delay_1s.isExpired() && !switchState) {
    for (led = 0; led < 10; ++led) {
      if (led == 10) {
        led = 0;
      }
      CircuitPlayground.setPixelColor(led, 0, 0, 255);
      delay(delaytime);
      CircuitPlayground.clearPixels();
      if (rightButtonFlag || leftButtonFlag) {
        delay(500);
        if (led == randLED) {
          CircuitPlayground.setPixelColor(led, 0 , 255 , 0);
          score++;
          Serial.println(score);
          CircuitPlayground.clearPixels();
          for (int i = 0; i < score; i++) {
            CircuitPlayground.setPixelColor(i, 0 , 255, 0);
            delay(200);
          }
          delay(500);
          delaytime = delaytime - 90;
          if (delaytime == 100){
            if (delay_1s.isExpired() && !switchState) {
              Serial.println("You win!");
              CircuitPlayground.playTone(500, 500);
              CircuitPlayground.clearPixels();
              delaytime = 1000;
              score = 0;
            }
          }
        }
        else {
          delay(500);
          CircuitPlayground.setPixelColor(led, 255 , 0 , 0);
          delay(500);
          Serial.println("You missed!");
        }
        CircuitPlayground.clearPixels();
        delay(500);
        led = 0;
        delay(100);
        CircuitPlayground.clearPixels();
        randLED = random(0,10);
        CircuitPlayground.setPixelColor(randLED, 255, 255, 255);
        delay(500);
        CircuitPlayground.clearPixels();
        rightButtonFlag = false;
        leftButtonFlag = false;
      }
    }
  }

  if(SwitchFlag) {
    delay(100);
    switchState = digitalRead(switchPin);
    CircuitPlayground.clearPixels();
    if (switchState == HIGH) {
      delay_1s.start(1000, AsyncDelay::MILLIS);
    }
    if(!switchState) {
      delaytime = 1000;
      delay(500);
      CircuitPlayground.clearPixels();
      delay(500);
      randLED = random(0, 10);
      CircuitPlayground.setPixelColor(randLED, 255, 255, 255);
      delay(500);
      CircuitPlayground.clearPixels();
    }
    SwitchFlag = false;
  }

      }
    

void switchISR() {
  SwitchFlag = true;
}

void rightbuttonISR() {
  rightButtonFlag = true;
}

void leftbuttonISR() {
  leftButtonFlag = true;
}