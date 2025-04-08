int signalPin = 2;
unsigned long highTime;
unsigned long lowTime;
float dutyCycle;

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT);
  Serial.println("Setup complete");
}

void loop() {
  int signalState = digitalRead(signalPin);

  highTime = pulseIn(signalPin, HIGH);
  lowTime = pulseIn(signalPin, LOW);

  if (highTime + lowTime > 0){
    dutyCycle = (highTime * 100.0) / (highTime + lowTime); 
    Serial.print("Duty Cycle: ");
    Serial.print(dutyCycle);
    Serial.println(" %");
  }

  delay(1000);
}