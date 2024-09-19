#include "Arduino.h"
#include "ESP32Servo.h"
#include "esp32-hal-gpio.h"
#include "esp32-hal.h"
#include "pins_arduino.h"
#include <cstdint>

inline const uint8_t SERVO_PIN = 18;
inline const uint8_t US_TRIGGER_PIN = 5;
inline const uint8_t US_ECHO_PIN = 21;

inline const uint8_t OPEN_ANGLE = 85;
inline const uint8_t CLOSE_ANGLE = 105;

Servo servo;

void openClaw(Servo *servo) {
  Serial.println("Abrindo garra");
  servo->write(OPEN_ANGLE);
  delay(1000);
}

void closeClaw(Servo *servo) {
  Serial.println("Fechando garra");
  servo->write(CLOSE_ANGLE);
  delay(1000);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(115200);
  pinMode(SERVO_PIN, OUTPUT);
  pinMode(US_TRIGGER_PIN, OUTPUT);
  pinMode(US_ECHO_PIN, INPUT);

  servo.attach(SERVO_PIN);
  delay(100);

  openClaw(&servo);

  closeClaw(&servo);

  openClaw(&servo);

  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  int duration;
  float distance;

  const uint8_t MIN_DISTANCE = 10;

  digitalWrite(US_TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(US_TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(US_TRIGGER_PIN, LOW);

  duration = pulseIn(US_ECHO_PIN, HIGH);
  distance = duration * 0.0344 / 2;

  (distance <= MIN_DISTANCE) ? closeClaw(&servo) : openClaw(&servo);

  Serial.printf("Dist: %f cm\n", distance);

  delay(500);
}
