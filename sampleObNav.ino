#include <NewPing.h> 


const int LeftMotorForward = 10;
const int LeftMotorBackward = 11;
const int RightMotorForward = 12;
const int RightMotorBackward = 13;


#define trig_pin 3 
#define echo_pin 2 

#define maximum_distance 200 
boolean goesForward = false; 
int distance = 100;          

NewPing sonar(trig_pin, echo_pin, maximum_distance); 

void setup() {
  
  pinMode(RightMotorForward, OUTPUT);
  pinMode(LeftMotorForward, OUTPUT);
  pinMode(LeftMotorBackward, OUTPUT);
  pinMode(RightMotorBackward, OUTPUT);

  
  delay(1000);
  distance = readPing();
}

void loop() {
  delay(50);

  if (distance <= 20) { 
    moveStop();           
    delay(300);           
    moveBackward();       
    delay(400);           
    moveStop();           
    delay(300);           
    turnRandom();         
  } else {
    moveForward();        
  }

  distance = readPing(); 
}


int readPing() {
  delay(70);
  int cm = sonar.ping_cm();
  if (cm == 0) { 
    cm = 250;
  }
  return cm;
}


void moveStop() {
  digitalWrite(RightMotorForward, LOW);
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);
  digitalWrite(LeftMotorBackward, LOW);
}


void moveForward() {
  if (!goesForward) {
    goesForward = true;
    digitalWrite(LeftMotorForward, HIGH);
    digitalWrite(RightMotorForward, HIGH);
    digitalWrite(LeftMotorBackward, LOW);
    digitalWrite(RightMotorBackward, LOW);
  }
}


void moveBackward() {
  goesForward = false;
  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorBackward, HIGH);
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorForward, LOW);
}


void turnRandom() {
  int randomDirection = random(0, 2); 
  
  if (randomDirection == 0) { 
    turnLeft();
  } else {
    turnRight();
  }
}


void turnRight() {
  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorBackward, HIGH);
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorForward, LOW);
  
  delay(600); 
  moveStop();
}


void turnLeft() {
  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorForward, HIGH);
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);
  
  delay(600); 
  moveStop();
}
