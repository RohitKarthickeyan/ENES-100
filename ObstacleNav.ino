#include "Enes100.h"
#include "Arduino.h"

//Right motor 
int in1 = 10;
int in2 = 11;

//Left Motor
int in3 = 12;
int in4 = 13;

const int TURN_PWM = 50;
const int MOVE_PWM = 50;
const float MOVE_TOLERANCE = 0.1;

const int trigPin = 3;  
const int echoPin = 2; 

float duration, distance;

void setup() {
  delay(2000);
  Enes100.begin("Anuskhanator", DATA, 499, 52, 50);
  delay(2000);
  Enes100.println("Vision System Initialized");

  
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(trigPin, OUTPUT);  
	pinMode(echoPin, INPUT);  
	Serial.begin(9600); 

}

void loop() {
  float x, y, t; bool v; 

  x = Enes100.getX();  
  y = Enes100.getY();  
  t = Enes100.getTheta();
  v = Enes100.isVisible();

  while(v){
    if(!detectObstacle()){
      setForward(30);
    }
    if(detectObstacle()){
      stopMotors();
      delay(1000);
      if(x>0.9 && x<2){
        if(y<1){
          Enes100.println("Obstacle Detected in Zone 3");
          turnLeftToAngle(PI/2);
          delay(1000);
          stopMotors();
          delay(1000);
        
          moveForward(Enes100.getX(), 0.8, 'Y');
          turnRightToAngle(0);
          stopMotors();
          delay(1000);
          //while(true);

        }else if(y>1){
          
          Enes100.println("Obstacle Detected in Zone 1");
          turnRightToAngle(-PI/2);
          delay(1000);
          stopMotors();
          delay(1000);
        
          moveForward(Enes100.getX(), 0.8, 'Y');
          turnLeftToAngle(0);
          stopMotors();
          delay(1000);
          //while(true);
        }
      }
      
    }
  }

}
void turnRightToAngle(float targetAngle) {
  
  while (abs(Enes100.getTheta() - targetAngle) > 0.5){
    turnRight(TURN_PWM);
  }
  
}
void turnLeftToAngle(float targetAngle){
  while (abs(Enes100.getTheta() - targetAngle) > 0.5){
    turnLeft(TURN_PWM);
  }

}
void moveForward(double targetXpos, double targetYpos, char axis) {

  unsigned long startTime = millis();
  unsigned long timeout = 10000;

  Enes100.println("About to Move Forward");
  setForward(MOVE_PWM);

  while ((axis == 'Y' && abs(Enes100.getY() - targetYpos) > MOVE_TOLERANCE) ||
          axis == 'X' && abs(Enes100.getX() - targetXpos) > MOVE_TOLERANCE)) {
    if (detectObstacle() || (millis() - startTime > timeout)) {
      stopMotors();
      return; // Exit if an obstacle or timeout occurs
    }
  }
  stopMotors();
  Enes100.println("Done moving forward");
}
void turnRight (float pwm) {

  digitalWrite(in1, LOW); 
  digitalWrite(in2, HIGH); 
  analogWrite(in2, pwm);
  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(in4, pwm);

  
}
void turnLeft (float pwm) {

  digitalWrite(in1, HIGH); 
  digitalWrite(in2, LOW); 
  analogWrite(in2, pwm);
  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(in4, pwm);

  
}
void setForward (float pwm) {

  digitalWrite(in1, HIGH); 
  digitalWrite(in2, LOW); 
  analogWrite(in1, pwm);
  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(in4, pwm);

  
}
void setBackwards (float pwm) {

  digitalWrite(in1, LOW); 
  digitalWrite(in2, HIGH); 
  analogWrite(in2, pwm);
  
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(in3, pwm);

  
}
void stopMotors() {

  digitalWrite(in1, LOW); 
  digitalWrite(in2, LOW); 
  analogWrite(in2, 0);

  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(in3, 0);
  
}
bool detectObstacle(){

  digitalWrite(trigPin, LOW);  
	delayMicroseconds(2);  
	digitalWrite(trigPin, HIGH);  
	delayMicroseconds(10);  
	digitalWrite(trigPin, LOW);  

  duration = pulseIn(echoPin, HIGH, 300);
  if (duration == 0) return false;

  distance = (duration*.0343)/2;

  return distance<10;
  
}
