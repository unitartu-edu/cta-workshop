/*
 * WRO Car (differential drive) — Arduino
 *
 * Motor pins are all PWM-capable (D3, D5, D6, D11) and avoid Timer1,
 * because the Servo library takes over Timer1 (PWM on D9/D10 stops working).
 *
 * Drive (L9110S)
 *   Left motor:   D5  A1A  |  D6  A1B
 *   Right motor:  D3  B1A  |  D11 B1B
 *
 * Ultrasonic sensors (HC-SR04)
 *   Right:  D2 Echo  |  D4 Trig
 *   Left:   D7 Echo  |  D8 Trig
 *
 * Pointer with SG90 servo (points the way the car is turning)
 *   Servo:  D9 Signal
 */

#include <Arduino.h>
#include <Servo.h>

// Pins
#define LEFT_A1A 5
#define LEFT_A1B 6
#define RIGHT_B1A 3
#define RIGHT_B1B 11
#define R_ECHO 2
#define R_TRIG 4
#define L_ECHO 7
#define L_TRIG 8
#define SERVO_PIN 9

// Config
#define THRESHOLD_CM  20
#define MOTOR_SPEED    120   // 0-255, where 255 is full speed forward.
#define TURN_SPEED      70   // slower speed for the wheel on the inside of a turn

#define SERVO_LEFT     120   // servo angle in degrees (0-180)
#define SERVO_STRAIGHT  90
#define SERVO_RIGHT     60

Servo pointerServo;

long getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 30000); // will time out after 30000 microseconds
  return duration * 0.034 / 2;
}

// Drive the left motor forward at the specified speed (0-255)
void leftForward(int speed) {
  digitalWrite(LEFT_A1B, LOW);
  analogWrite(LEFT_A1A, speed);
}

// Drive the right motor forward at the specified speed (0-255)
// (mounted mirrored, so it is driven through the other pin of the pair)
void rightForward(int speed) {
  digitalWrite(RIGHT_B1A, LOW);
  analogWrite(RIGHT_B1B, speed);
}

// This function only executes once.
void setup() {
  Serial.begin(9600);
  pinMode(R_TRIG, OUTPUT); pinMode(R_ECHO, INPUT);
  pinMode(L_TRIG, OUTPUT); pinMode(L_ECHO, INPUT);
  pinMode(LEFT_A1A, OUTPUT);
  pinMode(LEFT_A1B, OUTPUT);
  pinMode(RIGHT_B1A, OUTPUT);
  pinMode(RIGHT_B1B, OUTPUT);

  pointerServo.attach(SERVO_PIN);
  pointerServo.write(SERVO_STRAIGHT);

  // Kick-start: full power briefly to overcome static friction, otherwise
  // the motors can just whine at low PWM without turning.
  leftForward(255);
  rightForward(255);
  delay(200);
}

// This function executes repeatedly in a loop.
void loop() {
  long distRight = getDistance(R_TRIG, R_ECHO);
  long distLeft  = getDistance(L_TRIG, L_ECHO);

  Serial.print("Left: "); Serial.print(distLeft);
  Serial.print(" cm, Right: "); Serial.print(distRight);
  Serial.println(" cm");

  if (distLeft < THRESHOLD_CM) {
    // Wall on the left: slow down the right wheel to turn right
    leftForward(MOTOR_SPEED);
    rightForward(TURN_SPEED);
    pointerServo.write(SERVO_RIGHT);
  } else if (distRight < THRESHOLD_CM) {
    // Wall on the right: slow down the left wheel to turn left
    leftForward(TURN_SPEED);
    rightForward(MOTOR_SPEED);
    pointerServo.write(SERVO_LEFT);
  } else {
    leftForward(MOTOR_SPEED);
    rightForward(MOTOR_SPEED);
    pointerServo.write(SERVO_STRAIGHT);
  }

  delay(50);
}
