#include <ESP32Servo.h>
#include <NewPing.h>

// --- ESP32 Pin Assignments ---
#define TRIG_PIN 32
#define ECHO_PIN 33
#define SERVO_PIN 4

// --- L298N Motor Driver Pins ---
#define ENA 14
#define IN1 12
#define IN2 13

#define IN3 26
#define IN4 27
#define ENB 25

// --- PWM Channels ---
#define PWM_CHANNEL_A 0
#define PWM_CHANNEL_B 1
#define PWM_FREQ 5000
#define PWM_RESOLUTION 8

// --- Constants ---
#define MAX_DISTANCE 200
#define MAX_SPEED 200

#define SER_LEFT_POS 170
#define SER_MIDDLE_POS 110
#define SER_RIGHT_POS 50

#define DELAY_FORWARD 1500
#define DELAY_BACKWARD 750
#define DELAY_TURN 750
#define DELAY_STOP 100
#define DELAY_SERVO 500

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
Servo myservo;

int distance_Max_Obstacle = 35;
int distance_Left = 0;
int distance_Middle = 100;
int distance_Right = 0;

//------------------------ Setup ----------------------------
void setup()
{
  Serial.begin(115200);

  // Motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // PWM setup (Arduino-ESP32 core 2.x API)
  ledcSetup(PWM_CHANNEL_A, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(ENA, PWM_CHANNEL_A);

  ledcSetup(PWM_CHANNEL_B, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(ENB, PWM_CHANNEL_B);

  moveStop();

  // Servo setup
  myservo.setPeriodHertz(50);
  myservo.attach(SERVO_PIN, 500, 2400);

  myservo.write(SER_MIDDLE_POS);
  delay(DELAY_SERVO);

  distance_Middle = readPing();
}

//---------------------------- Main Loop ----------------------------
void loop()
{
  distance_Middle = readPing();

  if (distance_Middle <= distance_Max_Obstacle)
  {
    moveStop();
    delay(DELAY_STOP);

    moveBackward();
    delay(DELAY_BACKWARD);

    moveStop();
    delay(DELAY_STOP);

    distance_Right = lookRight();
    distance_Left = lookLeft();

    if (distance_Right >= distance_Left)
    {
      turnRight();
      delay(DELAY_TURN);
    }
    else
    {
      turnLeft();
      delay(DELAY_TURN);
    }

    moveStop();
  }
  else
  {
    moveForward();
  }

  Serial.println(distance_Middle);
}

//---------------------------- Movement Functions ----------------------------
void moveStop()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  ledcWrite(PWM_CHANNEL_A, 0);
  ledcWrite(PWM_CHANNEL_B, 0);
}

void moveForward()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  ledcWrite(PWM_CHANNEL_A, MAX_SPEED);
  ledcWrite(PWM_CHANNEL_B, MAX_SPEED);
}

void moveBackward()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  ledcWrite(PWM_CHANNEL_A, MAX_SPEED);
  ledcWrite(PWM_CHANNEL_B, MAX_SPEED);
}

void turnRight()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  ledcWrite(PWM_CHANNEL_A, MAX_SPEED);
  ledcWrite(PWM_CHANNEL_B, MAX_SPEED);
}

void turnLeft()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  ledcWrite(PWM_CHANNEL_A, MAX_SPEED);
  ledcWrite(PWM_CHANNEL_B, MAX_SPEED);
}

//---------------------------- Sensor Functions ----------------------------
int readPing()
{
  delay(50);
  int cm = sonar.ping_cm();

  // NewPing returns 0 when no echo is detected.
  // Treat this as beyond the configured maximum range.
  if (cm == 0)
    cm = 250;

  return cm;
}

int lookRight()
{
  myservo.write(SER_RIGHT_POS);
  delay(400);
  int distance = readPing();

  myservo.write(SER_MIDDLE_POS);
  delay(400);

  return distance;
}

int lookLeft()
{
  myservo.write(SER_LEFT_POS);
  delay(400);
  int distance = readPing();

  myservo.write(SER_MIDDLE_POS);
  delay(400);

  return distance;
}
