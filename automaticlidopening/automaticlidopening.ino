#include <Servo.h>

const int TRIG_PIN = 6;
const int ECHO_PIN = 9;
const int SERVO_PIN = 3;

Servo lidServo;

// This angle can be adjusted according to the actual trash bin design
const int LID_CLOSED_ANGLE = 180;    // Servo angle when the lid is closed
const int LID_OPEN_ANGLE   = 10;   // Servo angle when the lid is open

// 距离阈值（厘米）
const float OPEN_DISTANCE_CM  = 10.0;   // <= 10 cm: open the lid
const float CLOSE_DISTANCE_CM = 12.0;   // >= 12 cm: close the lid

bool lidIsOpen = false;

float getDistanceCm() {
 
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

 
  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // timeout 30ms

  if (duration == 0) {
    
    return 999.0;
  }

  
  float distanceCm = duration / 58.0;
  return distanceCm;
}

void setup() {
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  lidServo.attach(SERVO_PIN);
  lidServo.write(LID_CLOSED_ANGLE);  
}

void loop() {
  float distance = getDistanceCm();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (!lidIsOpen && distance <= OPEN_DISTANCE_CM) {
   
    lidServo.write(LID_OPEN_ANGLE);
    lidIsOpen = true;
    Serial.println("Lid OPEN");
    delay(300); 
  } 
  else if (lidIsOpen && distance >= CLOSE_DISTANCE_CM) {
    
    lidServo.write(LID_CLOSED_ANGLE);
    lidIsOpen = false;
    Serial.println("Lid CLOSE");
    delay(300);
  }

  delay(100); 
}
