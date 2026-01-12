#include <Servo.h>

Servo myservo;

// Pin definitions
const int servoPin = 3;
const int trigPin  = 6;
const int echoPin  = 9;

void setup() {
  Serial.begin(115200);

  // Servo setup
  myservo.attach(servoPin);
  myservo.write(90);  // center position
  delay(500);

  // Ultrasonic sensor setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.println("Test Started!");
}

long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000); // timeout 30ms
  long distance = duration * 0.034 / 2;

  return distance; // millimeters
}

void loop() {

  // Sweep 0 → 180 degrees
  for (int pos = 0; pos <= 180; pos += 5) {
    myservo.write(pos);
    delay(100);

    long dist = getDistance();
    Serial.print("Servo Pos: ");
    Serial.print(pos);
    Serial.print("°, Distance: ");
    Serial.print(dist);
    Serial.println(" cm");
  }

  // Sweep 180 → 0 degrees
  for (int pos = 180; pos >= 0; pos -= 5) {
    myservo.write(pos);
    delay(100);

    long dist = getDistance();
    Serial.print("Servo Pos: ");
    Serial.print(pos);
    Serial.print("°, Distance: ");
    Serial.print(dist);
    Serial.println(" cm");
  }
}

