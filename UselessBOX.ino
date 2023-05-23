// UselessBOX project by @artem_sann
// copyright UselessBOX, 2023
// this code is public domain, enjoy!

#include "AFMotor.h"     
#include <Servo.h>

AF_DCMotor motor3(3); 
AF_DCMotor motor4(4); 

Servo button_servo;
Servo cover_servo;

int pos_button = 0;
int pos_cover = 0;
bool state = 0;
float light = 0;
float start_light = 0;
char BTstate = 0;
int count = 0;

void setup() {
  Serial.begin(9600);
  motor3.setSpeed(255); 
  motor4.setSpeed(255); 
  motor3.run(RELEASE);   
  motor4.run(RELEASE);

  button_servo.attach(9);
  cover_servo.attach(10);
  
  pinMode(11, INPUT);
  start_light = analogRead(A5); 
}

void run_away() {
  motor3.run(BACKWARD);  
  motor4.run(BACKWARD);  
  delay(700);          
  motor3.run(FORWARD);  
  motor4.run(FORWARD);  
  delay(500);
  motor3.run(RELEASE);  
  motor4.run(RELEASE);  
}

void rotate() {
  motor3.run(BACKWARD);  
  motor4.run(FORWARD);  
  delay(600);         
  motor3.run(FORWARD);  
  motor4.run(BACKWARD);  
  delay(600); 
  motor3.run(BACKWARD);  
  motor4.run(FORWARD);  
  delay(600);          
  motor3.run(RELEASE);  
  motor4.run(RELEASE); 
}



void loop() {
  //-------------------Bluetooth control------------------------------------>
  BTstate = 0;
  if(Serial.available() > 0){     
      BTstate = Serial.read();  
       
    }

  if (BTstate == 'F') {
    motor3.run(RELEASE);
    motor4.run(RELEASE);
    motor3.run(BACKWARD);  
    motor4.run(BACKWARD);
  }
  else if (BTstate == 'B') {
    motor3.run(RELEASE);
    motor4.run(RELEASE);
    motor3.run(FORWARD);  
    motor4.run(FORWARD);
  }
  else if (BTstate == 'L') {
    motor3.run(RELEASE);
    motor4.run(RELEASE);
    motor3.run(BACKWARD);  
    motor4.run(FORWARD);
  }
  else if (BTstate == 'R') {
    motor3.run(RELEASE);
    motor4.run(RELEASE);
    motor3.run(FORWARD);  
    motor4.run(BACKWARD);
  }
  //------------------------------------------------------------------------->

  pos_button = 0;
  pos_cover = 0;
  button_servo.write(pos_button);

  delay(5);
  if (digitalRead(11) == 1 and count == 0) {
    delay(200);
    button_servo.write(180);
    delay(180);
    count += 1;
    button_servo.write(0);
  }
  delay(5);

  if ((digitalRead(11) == 1) and (count >= 1)) {
    delay(500);
    for (int i=0; i<60; i++) {
      button_servo.write(i);
      delay(7);
    }
    delay(400);
    button_servo.write(0);
    delay(1000);
    button_servo.write(180);
    delay(180);
    button_servo.write(0);
    count += 1;
  }
  delay(5);

  light = analogRead(A5);
  if (start_light - light > 150 and count == 2) {
    run_away();
    rotate();
    count = 0;
  }
 
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}
