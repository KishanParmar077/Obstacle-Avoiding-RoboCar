# Obstacle Avoiding RoboCar

An ESP32-based autonomous robot car that detects obstacles using an ultrasonic sensor and chooses a direction to avoid them. A servo motor turns the sensor to check the left and right sides before the car changes direction.

## Project overview

The robot continuously measures the distance in front of it using an HC-SR04 ultrasonic sensor. When an obstacle is detected within the configured threshold, the car stops, reverses, scans both sides, and turns toward the side with the greater measured distance. If the path is clear, it moves forward.

**Control flow:** Sense → Decide → Act

## Hardware components

- ESP32 development board
- HC-SR04 ultrasonic distance sensor
- L298N dual H-bridge motor driver
- Servo motor (sensor scanning)
- DC geared motors and wheels
- Robot chassis
- Battery pack and suitable power wiring
- Jumper wires

> Check the voltage and current requirements of your exact modules and motors before powering the robot. Do not power motors directly from an ESP32 GPIO pin.

## Pin connections

| Function | ESP32 GPIO |
|---|---:|
| HC-SR04 TRIG | 32 |
| HC-SR04 ECHO | 33 |
| Servo signal | 4 |
| L298N ENA (PWM) | 14 |
| L298N IN1 | 12 |
| L298N IN2 | 13 |
| L298N IN3 | 26 |
| L298N IN4 | 27 |
| L298N ENB (PWM) | 25 |

The motor driver's motor outputs connect to the DC motors. Connect grounds together (ESP32, motor driver, and sensor supply) so the control signals have a common reference.

**Important:** The HC-SR04 ECHO pin can output 5 V. ESP32 GPIOs are 3.3 V logic and are not 5 V tolerant. Use a suitable voltage divider or level shifter between ECHO and GPIO 33. Power the servo and motors from an appropriately rated supply; avoid drawing their current from the ESP32 3.3 V pin.

## Software and libraries

- Arduino IDE
- ESP32 board support package
- `ESP32Servo` library
- `NewPing` library

The sketch uses `ledcSetup()`, `ledcAttachPin()`, and channel-based `ledcWrite()` from the Arduino-ESP32 **2.x** LEDC API. If you use Arduino-ESP32 3.x, the LEDC API has changed and the PWM setup/write calls need to be adapted.

## How it works

1. The ESP32 initializes the motor-driver pins, PWM channels, servo, and ultrasonic sensor.
2. The ultrasonic sensor measures the distance in front of the robot.
3. If the measured distance is greater than 35 cm, the robot moves forward.
4. If an obstacle is detected at or within 35 cm, the robot stops and moves backward briefly.
5. The servo points the ultrasonic sensor right and then left to measure both directions.
6. The robot turns toward the side with the larger measured distance, then resumes obstacle detection.

If the ultrasonic sensor receives no echo, `NewPing` returns `0`; this sketch treats that reading as 250 cm (outside the configured sensor range).

## Adjustable settings

The main values can be changed near the top of the sketch:

| Setting | Current value | Purpose |
|---|---:|---|
| `distance_Max_Obstacle` | 35 cm | Obstacle detection threshold |
| `MAX_DISTANCE` | 200 cm | NewPing configured maximum distance |
| `MAX_SPEED` | 200 / 255 | PWM duty value (8-bit resolution) |
| `DELAY_BACKWARD` | 750 ms | Reverse duration |
| `DELAY_TURN` | 750 ms | Turning duration |
| `SER_LEFT_POS` | 170° | Servo left scan position |
| `SER_MIDDLE_POS` | 110° | Servo center position |
| `SER_RIGHT_POS` | 50° | Servo right scan position |

The actual turning angle and movement distance depend on the motor, chassis, battery level, wheel grip, and surface. Calibrate these values on your robot.

## Repository structure

```text
Obstacle-Avoiding-RoboCar/
├── Code/
│   └── Obstacle_Avoiding_RoboCar.ino
├── Images/
│   ├── project_front_view.jpg
│   └── project_top_view.jpg
└── README.md
```

## Project photos

### Front view
![Obstacle Avoiding RoboCar - front view](Images/project_front_view.jpg)

### Top view
![Obstacle Avoiding RoboCar - top view](Images/project_top_view.jpg)

## Uploading the code

1. Install Arduino IDE and add ESP32 board support.
2. Install the `ESP32Servo` and `NewPing` libraries from Library Manager.
3. Open `Code/Obstacle_Avoiding_RoboCar.ino`.
4. Select the correct ESP32 board and COM port.
5. Verify the pin connections and power arrangement before uploading.
6. Upload the sketch and test the robot in an open area.

## Future improvements

- Add smoother speed control and adjustable turning time.
- Improve distance sampling and obstacle decision logic.
- Add battery-voltage monitoring.
- Add a non-blocking control loop for more responsive operation.

## Author

**Kishan Parmar**

Electronics & Communication Engineering

---

This repository documents an educational prototype. Test it under supervision and verify wiring and power safety before operation.
