#include "FastLED.h"

int     pinI1     = 8;  //define I1 interface
int     pinI2     = 11; //define I2 interface 
int     speedpinA = 9;  //enable motor A
int     pinI3     = 12; //define I3 interface 
int     pinI4     = 13; //define I4 interface 
int     speedpinB = 10; //enable motor B
int     spead     = 127;//define the spead of motor

//#define NUM_LEDS 7 
//#define DATA_PIN 3

//CRGBArray<NUM_LEDS> leds;

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

/*
 * 
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
*/

void motorWorks() {
    analogWrite(speedpinA,left_speed);
    analogWrite(speedpinB,right_speed);

    switch (dir) {
      case 1:
        digitalWrite(pinI4,HIGH);//turn DC Motor B move clockwise
        digitalWrite(pinI3,LOW);
        digitalWrite(pinI2,LOW);//turn DC Motor A move anticlockwise
        digitalWrite(pinI1,HIGH);
        break;
      case 2:
        digitalWrite(pinI4,LOW);//turn DC Motor B move anticlockwise
        digitalWrite(pinI3,HIGH);
        digitalWrite(pinI2,HIGH);//turn DC Motor A move clockwise
        digitalWrite(pinI1,LOW);
        break;
      case 4:
        digitalWrite(speedpinA,LOW);// Unenble the pin, to stop the motor. this should be done to avid damaging the motor. 
        digitalWrite(speedpinB,LOW);
        break;        
    } 
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
    //LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
    //LEDS.setBrightness(84);
    pinMode(pinI1,OUTPUT);
    pinMode(pinI2,OUTPUT);
    pinMode(speedpinA,OUTPUT);
    pinMode(pinI3,OUTPUT);
    pinMode(pinI4,OUTPUT);
    pinMode(speedpinB,OUTPUT);
}


void loop() {
    
    if (Serial1.available()) {     
        
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
    
    //speedLed();
    motorWorks();
        
    //leds.fadeToBlackBy(1);
    //FastLED.show();
}


