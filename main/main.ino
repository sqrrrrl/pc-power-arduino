#include <ArduinoHttpClient.h>
#include "WiFiS3.h"
#include "environment.h"

const int POWER_CONTROL_PIN = 3;
const int RESET_CONTROL_PIN = 9;
const int LED_0 = 6; // LED-
const int LED_1 = 5; // LED+
const int PRESS_TIME = 400;
const int HARD_SHUTDOWN_PRESS_TIME = 5000;

const char ssid[] = SECRET_SSID;
const char pass[] = SECRET_PASS;

void setup() {
  resetControlPins();
}

void resetControlPins(){
  pinMode(POWER_CONTROL_PIN, INPUT);
  pinMode(RESET_CONTROL_PIN, INPUT)
}

void loop() {
  resetControlPins();
}

void pressPowerSwitch() {
  pinMode(POWER_CONTROL_PIN, OUTPUT);
  digitalWrite(POWER_CONTROL_PIN, LOW);
  delay(PRESS_TIME);
}

void pressResetSwitch(){
  pinMode(RESET_CONTROL_PIN, OUTPUT);
  digitalWrite(RESET_CONTROL_PIN, LOW);
  delay(PRESS_TIME);
}

void hardShutdown(){
  pinMode(POWER_CONTROL_PIN, OUTPUT);
  digitalWrite(POWER_CONTROL_PIN, LOW);
  delay(HARD_SHUTDOWN_PRESS_TIME);
}
