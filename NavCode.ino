#include "Enes100.h"
#include "Arduino.h"

//Right motor 
int in1 = 10;
int in2 = 11;

//Left Motor
int in3 = 12;
int in4 = 13;

//UltraSonic Sensor
int trigPin = 3;  
int echoPin = 2; 

const int TURN_PWM = 100;
const int MOVE_PWM = 160;
const float ANGLE_TOLERANCE = 0.7;
const float MOVE_TOLERANCE = 0.1;

void setup() {
  // Initialize the ENES100 Vision System with the provided parameter
  delay(2000);
  Enes100.begin("Anuskhanator", DATA, 499, 52, 50);
  delay(2000);
  Enes100.println("Vision System Initialized");

  Serial.begin(9600);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

  // check if the osv is visible from the vision system
  while (!Enes100.isVisible());
  Enes100.println("Visible");

  // Retrieve the starting y-coordinate
  float yCoordinate = Enes100.getY();
  Enes100.println(yCoordinate);

  // Determine which angle to turn based on the y-coordinate
  if (yCoordinate > 1.0) {
    turnToAngle(-PI / 2);  // Turn to -π/2 radians if y > 1
  } else {
    turnToAngle(PI / 2);  // Turn to π/2 radians if y < 1
  }
}

void loop() {
  if (Enes100.getY() < 1) {
    moveForward(Enes100.getX(), 1.4, 'Y');
    while (true)
      ;
  } else if (Enes100.getY() > 1) {
    moveForward(Enes100.getX(), 0.6, 'Y');
    while (true)
      ;
  }
}

void turnToAngle(float targetAngle) {

  digitalWrite(in1, LOW); 
  digitalWrite(in2, HIGH); 
  analogWrite(in2, TURN_PWM);
  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(in4, TURN_PWM);

  Enes100.print("Targeting angle: ");
  Enes100.println(targetAngle);

  while (abs(Enes100.getTheta() - targetAngle) > ANGLE_TOLERANCE) {
    // Enes100.println("Turning - " + String(Enes100.getTheta()));
  }

  Enes100.print("Arrived");

  /*
  Tank.setLeftMotorPWM(-TURN_PWM);
  Tank.setRightMotorPWM(TURN_PWM);
  delay(100); */

  stopMotors();
  Enes100.println("Done Turning");
}

void stopMotors() {

  digitalWrite(in1, LOW); 
  digitalWrite(in2, LOW); 
  analogWrite(in2, 0);

  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(in3, 0);
  //delay(1000);
}

void moveForward(double targetXpos, double targetYpos, char axis) {
  Enes100.println("About to Move Forward");

  if (axis == 'Y') {

    digitalWrite(in1, HIGH); 
    digitalWrite(in2, LOW); 
    analogWrite(in1, TURN_PWM);
  
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(in4, TURN_PWM);

    while (abs(Enes100.getY() - targetYpos) > MOVE_TOLERANCE) {
      Enes100.println("Moving Forward");
    }

    stopMotors();
    Enes100.println("Done moving forward");
  } 
  else if (axis == 'X') {
    
    digitalWrite(in1, HIGH); 
    digitalWrite(in2, LOW); 
    analogWrite(in1, TURN_PWM);
  
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(in4, TURN_PWM);

    while (abs(Enes100.getX() - targetXpos) > MOVE_TOLERANCE) {
      Enes100.println("Moving Forward"); 
    }

    stopMotors();
    Enes100.println("Done moving forward");
  }
}