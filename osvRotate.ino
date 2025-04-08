//Right motor 
int in1 = 10;
int in2 = 11;

//Left Motor
int in3 = 12;
int in4 = 13;

#include "Enes100.h"
#include "Arduino.h"

// Define target coordinates
const float A_X = 0.55;
const float A_Y = 0.45;
const float B_X = 0.55;
const float B_Y = 0.55;

//Define target angle
const float A_ANG = 1.5;
const float B_ANG = -1.5;



const int TURN_PWM = 70;

void setup() {
  
  delay(2000);
  Enes100.begin("Anuskhanator", DATA, 499, 52, 50);
  delay(2000);

  Serial.begin(9600);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  
}



void loop() {
  float x, y, t; bool v; 

  x = Enes100.getX();  
  y = Enes100.getY();  
  t = Enes100.getTheta();
  v = Enes100.isVisible();

  if(v){
    Enes100.print(x); 
    Enes100.print(",");
    Enes100.print(y);
    Enes100.print(",");
    Enes100.println(t);

    delay(1000);

    if (y > 1.0){
      turnToAngle(-PI/2);
    }
    else if (y < 1.0) {
      turnToAngle(PI / 2);  // Turn to π/2 radians if y < 1
      }
    
    delay(1000);

    digitalWrite(in1, LOW); 
    digitalWrite(in2, LOW); 
    analogWrite(in2, 0);
    
  
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    analogWrite(in3, 0);
    delay(11000);
    
  }


  //Tank.setLeftMotorPWM(255);
  //Tank.setRightMotorPWM(255);
}
void turnToAngle(float targetAngle) {
  while (abs(Enes100.getTheta() - targetAngle) > 0.4) {
    digitalWrite(in1, LOW); 
    digitalWrite(in2, HIGH); 
    analogWrite(in2, TURN_PWM);
  
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(in4, TURN_PWM);
  }
}

