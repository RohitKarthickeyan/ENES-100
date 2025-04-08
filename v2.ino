#include "Enes100.h"
#include "Arduino.h"
#include "Servo.h"

//Right motor 
int in1 = 10;
int in2 = 11;

//Left Motor
int in3 = 12;
int in4 = 13;

int enA = 6;
int enB = 7;

Servo myServo;

const int trigPin = 3;
const int echoPin = 2;

const int hallPin1 = 5;
const int hallPin2 = 4;

float duration, distance;

const int TURN_PWM = 100;
const int MOVE_PWM = 60;
const float MOVE_TOLERANCE = 0.1;

const float ttol = 0.1;

bool m1 = true;
bool m2 = false;
bool m3 = false;
bool m4 = false;

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
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  //myServo.attach(9);
  pinMode(hallPin1, INPUT);
  pinMode(hallPin2, INPUT);


  // check if the osv is visible from the vision system
  while (!Enes100.isVisible());
  Enes100.println("Visible");
}

void loop() {
  
  if(m1){
    if(Enes100.getY()>1){
        float targetX = 0.30;
        float targetY = 0.8; 
        navigateTo(targetX, targetY);
        setForward(150);
        while (Enes100.getY() > 0.5);
        stopMotors();
        m1 = false;
      }
      if(Enes100.getY()<1){
        float targetX = 0.28;
        float targetY = 1.2; 
        navigateTo(targetX, targetY);
        setForward(150);
        while (Enes100.getY() < 1.4);
        stopMotors();
        m1 = false;
        m3 = true;
      }
  }
  // the following code for the 2nd part (servo) should just not happen since m3 is set to true instead
  if(m2){
    for(int i = 0; i<3; i++){
      myServo.write(105); //speed of servo clockwise
      delay(3000); //time servo runs 
      myServo.write(90);
      delay(2000);
      myServo.write(75); //speed of servo counterclockwise
      delay(4000); //time servo runs
      //myServo.write(90);
      int hallState1 = digitalRead(hallPin1);
      int hallState2 = digitalRead(hallPin2);
      if (hallState1 == HIGH || hallState2 == HIGH) {
        Serial.print("Magnet Detected!");
        Enes100.mission(MAGNETISM, MAGNETIC);
      } else {
        Serial.print("Magnet Not Detected");
        Enes100.mission(MAGNETISM, MAGNETIC);
      }
    }
    
    m2 = false;
    m3 = true;

  }
  
  if(m3){
    setBackwards(76);
    avoidObstacles();
    Enes100.println("Perry is so done with ENES");
    Enes100.println("Perry wants to sleep");
    m3 = false;

  }
}

void avoidObstacles(){

  int i = 1;

  while(i == 1){
    navigateTo(1.01,0.99); // Site 1 Middle
    float q = Enes100.getTheta();
    
    turnToAngle2(0.00, q);//Look Forward
    Enes100.println("Looking Straight ahead");
    if(detectObstacle()){
      Enes100.println("Obstacle detected @ Middle Site!");
      setBackwards(70);
      delay(400);
      Enes100.println("Going to the top site!");
      navigateTo(0.83,1.73); //Site 1 Top
      turnToAngle2(0,Enes100.getTheta()); //Look forward
      Enes100.println("Looking Straight ahead");
      if(detectObstacle()){
        Enes100.println("Obstacle detected @ Top Site!");
        setBackwards(70);
        delay(400);
        Enes100.println("Going to Bottom Site");
        
        navigateTo(1.09,0.35);//Site 1 bottom
        
        navigateTo(1.78, 0.35);//Site 2 bottom
        turnToAngle2(0.00, Enes100.getTheta());
        Enes100.println("Looking Straight ahead");
        if(detectObstacle()){
          Enes100.println("Obstacle detected @ Bottom Site!");
          setBackwards(70);
          delay(400);
          Enes100.println("Going to Middle Site 2");
          
          navigateTo(1.70,0.98);//Site 2 middle
          
          navigateToFast(2.88, 0.98);// Site 3 middle
          
          navigateTo(2.88, 0.43);//Site 3 bottom
          
          turnToAngle2(0, Enes100.getTheta());
          setForward(200); // replaced navigate to fast here
          while(Enes100.getX() < 3.88);
          stopMotors();
          Enes100.println("Reached End of the Arena!");
          i=0;
          break;
        }
        else if(!detectObstacle()){// If there is no obstacle at Site 2 Bottom
          
          navigateToFast(2.88, 0.43);// Site 3 Bottom
          Enes100.println("Going Over the Log!");
          turnToAngle2(0, Enes100.getTheta());
          setForward(200); // replaced navigate to fast here
          while(Enes100.getX() < 3.88);
          stopMotors();
          Enes100.println("Reached End of the Arena!");
          i=0;
          break;

        }
      }
      else if(!detectObstacle()){// If there is no obstacle at Site 1 Top
        navigateTo(1.78, 1.59);// Site 2 Top
        turnToAngle2(0.00, Enes100.getTheta());
        if(detectObstacle()){
          navigateTo(1.70,0.98);// Site 2 Middle
          navigateToFast(2.88, 0.98);//Site 3 Middle
          navigateTo(2.88, 0.43);//Site 3 Bottom
          turnToAngle2(0, Enes100.getTheta());
          setForward(200); // replaced navigate to fast here
          while(Enes100.getX() < 3.88);
          stopMotors();
          Enes100.println("Reached End of the Arena!");
          i=0;
          break;
        }else if(!detectObstacle()){
          navigateToFast(2.88, 1.59);//Site 3 Top
          navigateTo(2.88, 0.43);//Site 3 Bottom
          turnToAngle2(0, Enes100.getTheta());
          setForward(200); // replaced navigate to fast here
          while(Enes100.getX() < 3.88);
          stopMotors();
          Enes100.println("Reached End of the Arena!");
          i=0;
          break;

        }

      }
    
    }
    else if(!detectObstacle()){
      
      navigateTo(1.78, 1.01);//Middle Site 2
      turnToAngle2(0, Enes100.getTheta());
      if(detectObstacle()){
        setBackwards(70);
        delay(400);
        navigateTo(1.78, 0.35);//Bottom Site 2
        
        navigateToFast(2.88, 0.43);//Bottom Site 3
        turnToAngle2(0, Enes100.getTheta());
        setForward(200); // should go forward continously 
        while(Enes100.getX() < 3.88);
        stopMotors();
        //navigateToFast(3.4, 0.43);//Bottom Site 4
        Enes100.println("Reached End of the Arena!");


        i=0;
        break;
      }
      if(!detectObstacle()){
        
        navigateToFast(2.88, 1.01);//Site 3 Middle
        navigateTo(2.88, 0.43);//Site 3 Bottom
        turnToAngle2(0, Enes100.getTheta());
        setForward(200); // replaced navigate to fast here
        while(Enes100.getX() < 3.88);
        stopMotors();
        Enes100.println("Reached End of the Arena!");
        i=0;
        break;
      }
    }
  }
}
int detectObstacle() {

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    duration = pulseIn(echoPin, HIGH);
    distance = (duration * .0343) / 2;
    
    return distance < 25;
    
}
void turnToAngle(float targetAngle, float angleRN){

  float stopBound = PI/40;
  float angleRn = Enes100.getTheta();
  
  
  bool isDone = abs(targetAngle - angleRn) <= stopBound;

  while(!isDone){
    angleRn = Enes100.getTheta();
    if(abs(targetAngle - angleRn) >= stopBound){
      Enes100.println(abs(targetAngle - angleRn));
      Enes100.println("Turning Right to Angle!");
      turnRight(TURN_PWM);
    }
    else if(abs(targetAngle - angleRn) <= stopBound){
      Enes100.println("Reached Target Angle!");
      stopMotors();
      isDone = true;
    }

    delay(200); 
    stopMotors();
    delay(500); 

  }
}
void turnToAngle2(float targetAngle, float currentAngle) {
    float stopBound = PI / 40; // Angular tolerance for stopping
    float angleRn = currentAngle; // Current angle
    bool isDone = false;

    while (!isDone) {
        angleRn = Enes100.getTheta(); // Continuously update the current angle
        float angleDiff = targetAngle - angleRn;

        // Normalize angleDiff to the range [-PI, PI]
        while (angleDiff > PI) angleDiff -= 2 * PI;
        while (angleDiff < -PI) angleDiff += 2 * PI;

        if (abs(angleDiff) > stopBound) {
            if (angleDiff > 0) {
                // Turn right if clockwise rotation is shorter
                Enes100.println("Turning Right to Angle: " + String(angleDiff));
                turnRight(TURN_PWM);
            } else {
                // Turn left if counterclockwise rotation is shorter
                Enes100.println("Turning Left to Angle: " + String(angleDiff));
                turnLeft(TURN_PWM);
            }
        } else {
            // Target angle reached
            Enes100.println("Reached Target Angle!");
            stopMotors();
            isDone = true;
        }

        // Add delays for vision system processing
        delay(200); // Allow the vision system to process the current angle
        stopMotors(); // Pause movement momentarily
        delay(500); // Allow the system to stabilize before checking again
    }
}
void navigateTo(float targetX, float targetY) {
    while (true) {
        if (!Enes100.isVisible()) {
            Enes100.println("Can't see the OSV!");
            stopMotors();
            delay(500);
            continue;
        }

        float currentX = Enes100.getX();
        float currentY = Enes100.getY();
        float currentTheta = Enes100.getTheta();

        float deltaX = targetX - currentX;
        float deltaY = targetY - currentY;
        float targetTheta = atan2(deltaY, deltaX);

        Enes100.println("targetTheta:");
        Enes100.println(targetTheta);

        float distance = sqrt(deltaX * deltaX + deltaY * deltaY);

        // Stop navigating if close enough
        if (distance <= MOVE_TOLERANCE) {
            stopMotors();
            Enes100.println("Arrived at Mission Site!");
            break;
        }

        // Adjust orientation first
        Enes100.println("Turning to Mission");
        turnToAngle2(targetTheta, currentTheta);

        // Move forward with proportional slowdown
        if (distance > 0.5) {
            Enes100.println("Mission Straight Ahead! (Fast)");
            setForward(90); // Normal speed
        } else if (distance > 0.2) {
            Enes100.println("Mission Straight Ahead! (Slow)");
            setForward(70); // Slower speed
        } else if (distance > 0.1){
            Enes100.println("Mission Straight Ahead! (Creep)");
            setForward(60); // Very slow speed
        }

        delay(300); // Time for movement
        stopMotors();
        delay(200); // Pause to stabilize
    }
}
void navigateToFast(float targetX, float targetY) {
    while (true) {
        if (!Enes100.isVisible()) {
            Enes100.println("Can't see the OSV!");
            stopMotors();
            delay(500);
            continue;
        }

        float currentX = Enes100.getX();
        float currentY = Enes100.getY();
        float currentTheta = Enes100.getTheta();

        float deltaX = targetX - currentX;
        float deltaY = targetY - currentY;
        float targetTheta = atan2(deltaY, deltaX);

        Enes100.println("targetTheta:");
        Enes100.println(targetTheta);

        float distance = sqrt(deltaX * deltaX + deltaY * deltaY);

        // Stop navigating if close enough
        if (distance <= MOVE_TOLERANCE) {
            stopMotors();
            Enes100.println("Arrived at Mission Site!");
            break;
        }

        // Adjust orientation first
        Enes100.println("Turning to Mission");
        turnToAngle2(targetTheta, currentTheta);

        // Move forward with proportional slowdown
        if (distance > 0.5) {
            Enes100.println("Mission Straight Ahead! (Fast)");
            setForward(100); // Normal speed
        } else if (distance > 0.2) {
            Enes100.println("Mission Straight Ahead! (Slow)");
            setForward(80); // Slower speed
        } else if (distance > 0.1){
            Enes100.println("Mission Straight Ahead! (Creep)");
            setForward(60); // Very slow speed
        }

        delay(600); // Time for movement
        stopMotors();
        delay(200); // Pause to stabilize
    }
}
void turnRight (int pwm) {
    digitalWrite(in1, LOW); 
    digitalWrite(in2, HIGH); 
    analogWrite(enA, pwm);
    
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(enB, pwm);
}

void turnLeft (int pwm) {
    digitalWrite(in1, HIGH); 
    digitalWrite(in2, LOW); 
    analogWrite(enA, pwm);
    
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enB, pwm);
}

void setForward (int pwm) {
    digitalWrite(in1, HIGH); 
    digitalWrite(in2, LOW); 
    analogWrite(enA, pwm);
    
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(enB, pwm);
}

void setBackwards (int pwm) {
    digitalWrite(in1, LOW); 
    digitalWrite(in2, HIGH); 
    analogWrite(enA, pwm);
    
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enB, pwm);
}

void stopMotors() {
    digitalWrite(in1, LOW); 
    digitalWrite(in2, LOW); 
    analogWrite(enA, 0);

    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    analogWrite(enB, 0);
}