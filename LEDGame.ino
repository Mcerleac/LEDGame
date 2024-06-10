#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>

AsyncDelay delay_1s; //All the varibales
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
  Serial.begin(9600) //Starts the serial monotor 
  randomSeed(analogRead(0));
  attachInterrupt(digitalPinToInterrupt(7), switchISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(4), rightbuttonISR, RISING);
  attachInterrupt(digitalPinToInterrupt(5), leftbuttonISR, RISING);
  switchState = digitalRead(switchPin);
  delay_1s.start(1000, AsyncDelay::MILLIS);
  if (!switchState) { //When the switch is flipped start the game.
    delaytime = 1000;
  }
  delay(500);
  CircuitPlayground.clearPixels();
  delay(500);
  randLED = random(0, 10);
  CircuitPlayground.setPixelColor(randLED, 255, 255, 255); //Set's the random pixel
  delay(500);
  CircuitPlayground.clearPixels(); //Clear's the random pixel
}

void loop() {
  if (delay_1s.isExpired() && !switchState) {
    for (led = 0; led < 10; ++led) { //Cycle's through the led circle
      if (led == 10) { //If for whatever reason led hits the 10th one goes back to 0
        led = 0;
      }
      CircuitPlayground.setPixelColor(led, 0, 0, 255);
      delay(delaytime);
      CircuitPlayground.clearPixels();
      if (rightButtonFlag || leftButtonFlag) { //If button is hit
        delay(500);
        if (led == randLED) { //If the button is hit when the led is on the random led then give player poinrs and print scoreboard
          CircuitPlayground.setPixelColor(led, 0 , 255 , 0);
          score++;
          Serial.println(score);
          CircuitPlayground.clearPixels();
          for (int i = 0; i < score; i++) {
            CircuitPlayground.setPixelColor(i, 0 , 255, 0);
            delay(200);
          }
          delay(500);
          delaytime = delaytime - 90; //Lower the delay time by 90 ms
          if (delaytime == 100){ //If the delay time is 100 then the user wins, so it plays a tone and restarts the game
            if (delay_1s.isExpired() && !switchState) {
              Serial.println("You win!");
              CircuitPlayground.playTone(500, 500);
              CircuitPlayground.clearPixels();
              delaytime = 1000;
              score = 0;
            }
          }
        }
        else {//If the user misses, turn the led red and print you missed, then go again
          delay(500);
          CircuitPlayground.setPixelColor(led, 255 , 0 , 0);
          delay(500);
          Serial.println("You missed!");
        }
        CircuitPlayground.clearPixels(); //Refresh the board and random led. 
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

  if(SwitchFlag) { //If switch is flipped, stop the game
    delay(100);
    switchState = digitalRead(switchPin);
    CircuitPlayground.clearPixels();
    if (switchState == HIGH) {
      delay_1s.start(1000, AsyncDelay::MILLIS);
    }
    if(!switchState) {//Once switch is flippled again restart the game.
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
