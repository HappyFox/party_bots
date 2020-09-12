
#include <Arduino.h>
#include <ServoEasing.h>

#define LEFT_SERVO_PIN D4 // D4
#define RIGHT_SERVO_PIN D3 // D3
#define RESET_SERVO_PIN D2 // D4

#define MID_POINT 90

ServoEasing LeftServo;
ServoEasing RightServo;
ServoEasing ResetServo;


void setup() {
    if (LeftServo.attach(LEFT_SERVO_PIN) == INVALID_SERVO) {
        Serial.println(F("Error attaching Left arm servo"));
    }
    if (RightServo.attach(RIGHT_SERVO_PIN) == INVALID_SERVO) {
        Serial.println(F("Error attaching Right arm servo"));
    }
    if (ResetServo.attach(RESET_SERVO_PIN) == INVALID_SERVO) {
        Serial.println(F("Error attaching Reset servo"));
    }
    ResetServo.setSpeed(120);
    ResetServo.startEaseTo(MID_POINT);
    RightServo.setSpeed(120);
    RightServo.startEaseTo(MID_POINT);
    LeftServo.setSpeed(120);
    LeftServo.startEaseTo(MID_POINT);
}

void loop() {
}
