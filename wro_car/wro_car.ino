/*
 * WRO Car — Arduino
 *
 * Ultrasonic sensors (HC-SR04)
 *   Right:  D2 Echo  |  D3 Trig
 *   Left:   D4 Echo  |  D5 Trig
 *
 * Steering with SG90 servo
 *   Servo:  D6 Signal
 *
 * Drive (L9110S)
 *   Rear:   D10 A1A  |  D11 A1B
 */

#include <Arduino.h>
#include <Servo.h>

// Pins
// Right ultrasonic sensor
#define R_ECHO 2
#define R_TRIG 3
#define L_ECHO 4
#define L_TRIG 5
#define SERVO_PIN 6
#define MOTOR_A1A 10
#define MOTOR_A1B 11

// Config
#define THRESHOLD_CM  20
#define MOTOR_SPEED     60   // 0-255, where 255 is full speed forward.

#define SERVO_TURN     30
#define SERVO_STRAIGHT 90
#define SERVO_LEFT     (SERVO_STRAIGHT - SERVO_TURN)
#define SERVO_RIGHT    (SERVO_STRAIGHT + SERVO_TURN)

Servo steeringServo;

long getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 30000); // will time out after 30000 microseconds
  return duration * 0.034 / 2;
}

// Drive forward at the specified speed (0-255)
void forward(int speed) {
  digitalWrite(MOTOR_A1A, HIGH);
  analogWrite(MOTOR_A1B, 255 - speed);
}

// Stop the car
void stop() {
  digitalWrite(MOTOR_A1A, LOW);
  digitalWrite(MOTOR_A1B, LOW);
}

// This function only executes once.
void setup() {
  //Serial.begin(9600);
  pinMode(R_TRIG, OUTPUT); pinMode(R_ECHO, INPUT);
  pinMode(L_TRIG, OUTPUT); pinMode(L_ECHO, INPUT);
  pinMode(MOTOR_A1A, OUTPUT);
  pinMode(MOTOR_A1B, OUTPUT);

  // Next line increases the PWM frequency to inaudible levels (~62.5kHz)
  // by setting timer2 prescaler to 1
  //TCCR2B = (TCCR2B & 0b11111000) | 0x01;

  steeringServo.attach(SERVO_PIN);
  steeringServo.write(SERVO_STRAIGHT);
}

// This function executes repeatedly in a loop.
void loop() {
  long distRight = getDistance(R_TRIG, R_ECHO);
  long distLeft  = getDistance(L_TRIG, L_ECHO);

  
  if (distLeft < THRESHOLD_CM) {
    steeringServo.write(SERVO_RIGHT);
  } else if (distRight < THRESHOLD_CM) {
    steeringServo.write(SERVO_LEFT);
  } else {
    steeringServo.write(SERVO_STRAIGHT);
  }
  

  // Drive forward at ~30% PWM
  forward(MOTOR_SPEED);

  delay(50);

  //Serial.print("Left: "); Serial.print(distLeft);
  //Serial.print(" cm, Right: "); Serial.print(distRight);
  //Serial.println(" cm");
}
