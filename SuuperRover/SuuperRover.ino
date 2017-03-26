#include "FastLED.h"
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
    
#define NUM_LEDS 7 
#define DATA_PIN 12

CRGBArray<NUM_LEDS> leds;

String value;

int    left_speed    = 0;
int    right_speed   = 0;
int    left_current  = 0;
int    right_current = 0;

int    dir = 4;

int    ledTrail     = 0;
int    delayTrail   = 0;

bool   debug = false;

int    data_in = 0;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *motor2 = AFMS.getMotor(2);
Adafruit_DCMotor *motor3 = AFMS.getMotor(3);
Adafruit_DCMotor *motor4 = AFMS.getMotor(4);

void speedLed() {
    delayTrail += 1;
    if (delayTrail == 50){
        delayTrail = 0;
        if (ledTrail == 8){
            ledTrail = 0;
        }        
        leds[ledTrail] = CRGB( 255,15,15);
        ledTrail += 1;
    }
    
    if (left_speed != 0){
        if (dir == 0){
            leds[2] = CRGB( 0, left_current, 0);
            leds[3] = CRGB( 0, left_current, 0);  
        } else {
            leds[2] = CRGB( 0, 0, right_current);
            leds[3] = CRGB( 0, 0, right_current);        
        }
    };
  
    if (right_speed != 0) {
        if (dir == 0){
            leds[5] = CRGB( 0, right_current, 0);
            leds[6] = CRGB( 0, right_current, 0);
        } else {
            leds[5] = CRGB( 0, 0, right_current);
            leds[6] = CRGB( 0, 0, right_current);
        }
    };
}

void motorWorks() {
    motor1->setSpeed(left_speed);
    motor1->run(dir);
    motor3->setSpeed(left_speed);
    motor3->run(dir);
    motor2->setSpeed(right_speed);
    motor2->run(dir);
    motor4->setSpeed(right_speed);
    motor4->run(dir);
  
}

void debuggy() {
  Serial.print("left: ");
  Serial.print(left_speed);
  Serial.print(" right: ");
  Serial.print(right_speed);
  Serial.print(" Direction: ");
  Serial.println(dir);
}

void setup() {
    Serial.begin(115200);
    Serial1.begin(19200);
    LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
    LEDS.setBrightness(84);
    AFMS.begin();
}


void loop() {
    
    if (Serial1.available()) {     // If anything comes in Serial1 (pins 0 & 1)
        
        value = Serial1.readStringUntil('|');
        Serial.println(value);
        data_in += 1;
        
        switch ( value.substring(0,1).toInt() ){
            case 1:
              left_speed  = value.substring(2).toInt();
              break;
            case 2:
              right_speed = value.substring(2).toInt();
              break;
            case 3:
              dir = value.substring(2).toInt();
              break;
        }
    }

    EVERY_N_SECONDS( 5 ) {   Serial.print("Serial commands receive per second:"); Serial.println(data_in / 5); data_in = 0; }
    
    speedLed();
    motorWorks();
    //debuggy();
    
    leds.fadeToBlackBy(1);
    FastLED.show();
}


