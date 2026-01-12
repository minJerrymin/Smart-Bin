#include <Servo.h>
#include "Adafruit_VL53L0X.h"

const int TRIG_PIN   = 6;
const int ECHO_PIN   = 9;
const int SERVO_PIN  = 3;
const int BUZZER_PIN = 5;  

Servo lidServo;

// This angle can be adjusted according to the actual trash bin design
const int LID_CLOSED_ANGLE = 180;  // Servo angle when the lid is closed
const int LID_OPEN_ANGLE   = 10;   // Servo angle when the lid is open

const float OPEN_DISTANCE_CM  = 10.0;   // <= 10 cm: open the lid
const float CLOSE_DISTANCE_CM = 12.0;   // >= 12 cm: close the lid

bool lidIsOpen = false;

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

// Total depth of the trash bin
const float BIN_DEPTH_CM = 21.0;

// Usage alarm thresholds
const float FULL_THRESHOLD_PERCENT  = 80.0;  // Alarm when above 80%
const float RESET_THRESHOLD_PERCENT = 70.0;  // Allow alarm again when below 70%

bool hasAlarmed = false;   

// Function: Ultrasonic distance measurement (from Arduino library)
float getDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // timeout 30 ms

  if (duration == 0) {
    // If no reading is obtained, return a large value
    return 999.0;
  }

  // Speed of sound is approximately 340 m/s.
  //Distance (m) = time × speed.
  //Converted: distance (cm) = time (µs) / 58.
  float distanceCm = duration / 58.0;
  return distanceCm;
}

// Function: Buzzer alarm
void beepFor5Seconds() {
  tone(BUZZER_PIN, 1000);  // 1 kHz tone
  delay(5000);
  noTone(BUZZER_PIN);
}

// Function: Read trash bin usage percentage
// Returns -1 if measurement fails
float getBinUsagePercent() {
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false); 

  if (measure.RangeStatus == 4) {
    return -1.0;
  }

  // Remaining depth inside the bin
  float depthCm = measure.RangeMilliMeter / 10.0;

  float garbageHeightCm = BIN_DEPTH_CM - depthCm;

  if (garbageHeightCm < 0)            garbageHeightCm = 0;
  if (garbageHeightCm > BIN_DEPTH_CM) garbageHeightCm = BIN_DEPTH_CM;

  float usagePercent = garbageHeightCm * 100.0 / BIN_DEPTH_CM;
  return usagePercent;
}

void setup() {
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  lidServo.attach(SERVO_PIN);
  lidServo.write(LID_CLOSED_ANGLE);   

  // Initialize VL53L0X
  if (!lox.begin()) {
    Serial.println("Failed to find VL53L0X sensor, check wiring!");
    while (1) { delay(10); } // Stop here if the sensor is not found
  }

  Serial.println("Smart bin ready.");
}

void loop() {

  float distance = getDistanceCm();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (!lidIsOpen && distance <= OPEN_DISTANCE_CM) {
    // Person approaches, open the lid
    lidServo.write(LID_OPEN_ANGLE);
    lidIsOpen = true;
    Serial.println("Lid OPEN");
    delay(300); // Buffer to prevent repeated triggering
  } 
  else if (lidIsOpen && distance >= CLOSE_DISTANCE_CM) {
    // Person leaves, close the lid
    lidServo.write(LID_CLOSED_ANGLE);
    lidIsOpen = false;
    Serial.println("Lid CLOSE");
    delay(300);
  }

  // Read trash bin usage and check if alarm is needed
  float usagePercent = getBinUsagePercent();

  if (usagePercent < 0) {
    Serial.println("Bin depth: OUT OF RANGE");
  } else {
    Serial.print("Bin usage: ");
    Serial.print(usagePercent);
    Serial.println(" %");

    // Alarm logic
    if (!hasAlarmed && usagePercent >= FULL_THRESHOLD_PERCENT) {
      Serial.println("Bin >= 80%, ALARM 5s!");
      beepFor5Seconds();
      hasAlarmed = true;  // Mark that the alarm has been triggered
    } 
    else if (hasAlarmed && usagePercent <= RESET_THRESHOLD_PERCENT) {
      Serial.println("Bin usage back below reset threshold, alarm reset.");
      hasAlarmed = false;
    }
  }

  delay(200);  
}
