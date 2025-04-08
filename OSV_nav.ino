#include "Enes100.h"
#include "Arduino.h"

//Right motor
int in1 = 10;
int in2 = 11;

//Left Motor
int in3 = 12;
int in4 = 13;

const int TURN_PWM = 50;
const int MOVE_PWM = 100;
const float ANGLE_TOLERANCE = 0.25;
const float MOVE_TOLERANCE = 0.1;

void setup() {
  // Initialize the ENES100 Vision System with the provided parameter
  delay(2000);
  Enes100.begin("Anuskhanator", DATA, 499, 52, 50);
  delay(2000);
  Enes100.println("Vision System Initialized");

  // initialize all the osv motor pins
  Serial.begin(9600);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

  // check if the osv is visible from the vision system
  while (!Enes100.isVisible());
  Enes100.println("Visible");
}

void loop() {
  // Retrieve the starting y-coordinate
  float yCoordinate = Enes100.getY();
  Enes100.println(yCoordinate);

  // Determine which angle to turn based on the y-coordinate
  if (yCoordinate > 1.0) {
    turnToAngle(PI / 2);  // Turn to -π/2 radians if y > 1
  } else {
    turnToAngle(-PI / 2);  // Turn to π/2 radians if y < 1
  }

  // Determine which y-coordinate we need to move to depending on the starting y-coordinate
  if (Enes100.getY() < 1) {
    moveOSV(Enes100.getX(), 1.4, 'Y', "Backward");  // move up the map if we start down
    while (true);
  } 
  else if (Enes100.getY() > 1) {
    moveOSV(Enes100.getX(), 0.6, 'Y', "Backward");  // move down the map if we start up
    while (true);
  }
}

void turnToAngle(float targetAngle) {

  turnRight(TURN_PWM);  // starts all the motors

  Enes100.print("Targeting angle: ");
  Enes100.println(targetAngle);

  while (abs(Enes100.getTheta() - targetAngle) > ANGLE_TOLERANCE)
    ;  // check that we haven't reached the target angle yet

  Enes100.print("Arrived");

  stopMotors();  // stops the motors
  Enes100.println("Done Turning");
}

void moveOSV(double targetXpos, double targetYpos, char axis, String direction) {

  if (direction == "Forward") {
    Enes100.println("About to move forward");

    if (axis == 'Y') {

      while (abs(Enes100.getY() - targetYpos) > MOVE_TOLERANCE) {
        Enes100.println("Moving Forward");
        mForward(MOVE_PWM);
      }

      stopMotors();
      Enes100.println("Done moving forward");
    } else if (axis == 'X') {

      while (abs(Enes100.getX() - targetXpos) > MOVE_TOLERANCE) {
        Enes100.println("Moving Forward");
        mForward(MOVE_PWM);
      }

      stopMotors();
      Enes100.println("Done moving forward");
    }
  }
  else {
    Enes100.println("About to move backward");

    if (axis == 'Y') {

      while (abs(Enes100.getY() - targetYpos) > MOVE_TOLERANCE) {
        Enes100.println("Moving Backward");
        mBackward(MOVE_PWM);
      }

      stopMotors();
      Enes100.println("Done moving backward");
    } else if (axis == 'X') {

      while (abs(Enes100.getX() - targetXpos) > MOVE_TOLERANCE) {
        Enes100.println("Moving backward");
        mBackward(MOVE_PWM);
      }

      stopMotors();
      Enes100.println("Done moving backward");
    }
  }
}

void turnRight(float pwm) {

  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(in2, pwm);

  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(in4, pwm);
}

void turnLeft(float pwm) {

  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(in2, -pwm);

  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(in4, pwm);
}

void mForward(float pwm) {

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(in1, pwm);

  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(in4, pwm);
}

void mBackward(float pwm) {

  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(in1, pwm);

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(in4, pwm);
}

void stopMotors() {

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(in2, 0);

  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(in3, 0);
  delay(1000);
}