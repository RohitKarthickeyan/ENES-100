#include <Servo.h>

Servo myServo;  // Create a Servo object

void setup() {
  myServo.attach(9);  // Attach the servo on pin 9
}

void loop() {
  // Move the servo from 0 to 180 degrees
  /*for (int pos = 0; pos <= 180; pos += 1) {
    myServo.write(pos);  // Tell the servo to go to position in variable 'pos'
    delay(1000);           // Wait 15 ms for the servo to reach the position
  }*/
  myServo.write(105); //speed of servo clockwise
  delay(1000); //time servo runs 
  myServo.write(80); //speed of servo counterclockwise
  delay(1000); //time servo runs

}
