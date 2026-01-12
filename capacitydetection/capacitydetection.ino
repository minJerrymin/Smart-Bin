#include "Adafruit_VL53L0X.h"


const int BUZZER_PIN = 5;  


Adafruit_VL53L0X lox = Adafruit_VL53L0X();

// Total depth of the trash bin
const float BIN_DEPTH_CM = 21.0;

// Usage alarm thresholds
const float FULL_THRESHOLD_PERCENT  = 80.0;  // Alarm when above 80%
const float RESET_THRESHOLD_PERCENT = 70.0;  // Allow alarm again when below 70%


bool hasAlarmed = false;   



// Function: Buzzer alarm
void beepFor5Seconds() {
  tone(BUZZER_PIN, 1000);  
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

  // Current remaining depth within the bin (distance from sensor to waste surface)
  float depthCm = measure.RangeMilliMeter / 10.0;  // mm -> cm

  // Waste height = Total depth - Remaining depth
  float garbageHeightCm = BIN_DEPTH_CM - depthCm;

  if (garbageHeightCm < 0)              garbageHeightCm = 0;
  if (garbageHeightCm > BIN_DEPTH_CM)   garbageHeightCm = BIN_DEPTH_CM;

  float usagePercent = garbageHeightCm * 100.0 / BIN_DEPTH_CM;
  return usagePercent;
}



void setup() {
  Serial.begin(9600);

  pinMode(BUZZER_PIN, OUTPUT);


  if (!lox.begin()) {
    Serial.println("Failed to find VL53L0X sensor, check wiring!");
    while (1) { delay(10); } 
  }

  Serial.println("VL53L0X + Buzzer ready.");
}

void loop() {
  // Read trash bin usage and check if alarm is needed
  float usagePercent = getBinUsagePercent();

  if (usagePercent < 0) {
    Serial.println("Bin depth: OUT OF RANGE");
  } else {
    Serial.print("Bin usage: ");
    Serial.print(usagePercent);
    Serial.println(" %");

    // Read trash bin usage and check if alarm is needed
    if (!hasAlarmed && usagePercent >= FULL_THRESHOLD_PERCENT) {
      Serial.println("Bin >= 85%, ALARM 5s!");
      beepFor5Seconds();
      hasAlarmed = true;  
    } 
    else if (hasAlarmed && usagePercent <= RESET_THRESHOLD_PERCENT) {
      
      Serial.println("Bin usage back below reset threshold, alarm reset.");
      hasAlarmed = false;
    }
  }

  delay(500);
}
