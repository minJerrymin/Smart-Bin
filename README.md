# Smart Bin

Smart Bin is a sensor-based interactive waste monitoring system.  
The project combines automatic lid opening with capacity detection to improve hygiene and usability in everyday waste disposal.

A reliable, practical desktop waste storage solution


---

## Project Overview

Smart Bin is a desktop-scale smart waste bin controlled by an Arduino Uno.  
It uses multiple sensors to detect user presence and monitor the internal capacity of the bin.  
When a user approaches, the lid opens automatically. When the bin reaches a predefined capacity threshold, an audible alert is triggered.

The project is intended as a proof-of-concept demonstrating how low-cost sensors and microcontrollers can be used to enhance everyday objects.

---

## Hardware Components

- Arduino Uno R3  
- HC-SR04 Ultrasonic Sensor (user distance detection)  
- VL53L0X Time-of-Flight Distance Sensor (capacity detection)  
- MG90S Servo Motor (lid actuation)  
- Buzzer (capacity alert)  
- 18650 Battery + Boost Converter (5V power supply)  
- Breadboard and jumper wires  
- 3D-printed enclosure  

---

## System Functions

### Automatic Lid Opening
- The ultrasonic sensor detects user presence in front of the bin.
- When the distance is below a threshold, the servo motor opens the lid.
- When the user moves away, the lid closes automatically.

### Capacity Detection and Alarm
- The VL53L0X sensor measures the remaining depth inside the bin.
- The system calculates the percentage of bin usage.
- When usage exceeds a set threshold, the buzzer sounds for 5 seconds.
- A reset threshold prevents repeated alarms.

---

## Software

The system was developed using the Arduino IDE.  
The final integrated Arduino sketch can be found in the `final_code` folder.

The program runs in a continuous loop and uses modular functions to handle:
- Distance measurement
- Capacity calculation
- Lid control
- Alarm triggering

---

## Circuit Diagram

The following diagram shows the wiring between the Arduino, sensors, servo motor and power supply.

![Smart Bin Circuit Diagram](https://github.com/minJerrymin/Smart-Bin/blob/1100ad29c516e3bc90a3b9562d42e7ae5237bc17/fritzing%20circuit%20diagram.png)

---

## Demonstration Videos

The repository includes demonstration videos showing system operation:

- `lid opening.mp4` – automatic lid opening and closing  
- `capacity.mp4` – capacity detection and alarm triggering  

---

---

## Author

**Tianrui Min**  
CASA0016 – Connected Environments  
University College London
