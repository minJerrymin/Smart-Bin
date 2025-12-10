#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(100);

  if (!sensor.init()) {
    Serial.println("Failed to detect and initialize VL53L0X!");
    while (1);
  }

  sensor.setMeasurementTimingBudget(20000); 

  Serial.println("VL53L0X init OK!");
}

void loop() {
  uint16_t distance = sensor.readRangeSingleMillimeters();

  if (sensor.timeoutOccurred()) {
    Serial.println("Timeout!");
  } else {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" mm");
  }

  delay(100);
}
