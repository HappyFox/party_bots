
#include <Arduino.h>
#include <ServoEasing.h>

#define RESET_SERVO_PIN D2 // D4

#define MID_POINT 90
#define RESET_POINT 140
#define RESET_SPEED 100

ServoEasing ResetServo;


void setup() {
    if (ResetServo.attach(RESET_SERVO_PIN) == INVALID_SERVO) {
        Serial.println(F("Error attaching Reset servo"));
    }
    ResetServo.setSpeed(RESET_SPEED);
    ResetServo.setEasingType(EASE_QUADRATIC_OUT);
    //ResetServo.setEasingType(EASE_QUARTIC_OUT);
    //ResetServo.setEasingType(EASE_CUBIC_OUT);
    ResetServo.write(MID_POINT);
    //ResetServo.startEaseTo(MID_POINT);
}

void loop() {
    ResetServo.easeTo(MID_POINT);
    delay(500);
    ResetServo.easeTo(RESET_POINT);
}
