#include "PersonalityImpl.h"

// Servo control helper functions
static void moveServoToAngle(uint8_t pin, uint8_t angle, unsigned long duration) {
    // Map angle (0-180) to PWM value (1000-2000 microseconds for SG90)
    unsigned long pulseWidth = 1000 + (angle * 1000 / 180);
    unsigned long startTime = Platform::millis();

    while (Platform::millis() - startTime < duration) {
        Platform::digitalWrite(pin, 1); // HIGH
        Platform::delayMicroseconds(pulseWidth);
        Platform::digitalWrite(pin, 0); // LOW
        Platform::delayMicroseconds(20000 - pulseWidth);
    }
}

// Fast personality - quick flick
void FastPersonality::execute(uint8_t servoPin) {
    // Move to 90 degrees (off position) quickly
    moveServoToAngle(servoPin, 90, 300);
    // Return to 0 degrees (on position)
    moveServoToAngle(servoPin, 0, 200);
}

// Slow personality - deliberate and methodical
void SlowPersonality::execute(uint8_t servoPin) {
    // Slowly move to 90 degrees
    moveServoToAngle(servoPin, 90, 1500);
    // Slowly return to 0 degrees
    moveServoToAngle(servoPin, 0, 500);
}

// Jittery personality - nervous twitches
void JitteryPersonality::execute(uint8_t servoPin) {
    // Quick twitch up
    moveServoToAngle(servoPin, 45, 100);
    Platform::delay(100);
    // Twitch back
    moveServoToAngle(servoPin, 0, 100);
    Platform::delay(100);
    // Another twitch
    moveServoToAngle(servoPin, 60, 150);
    Platform::delay(100);
    // Final movement to off position
    moveServoToAngle(servoPin, 90, 300);
    // Return
    moveServoToAngle(servoPin, 0, 200);
}

// Lazy personality - reluctant and slow
void LazyPersonality::execute(uint8_t servoPin) {
    // Slow, lazy movement to off position
    moveServoToAngle(servoPin, 90, 2000);
    Platform::delay(500);
    // Slow return
    moveServoToAngle(servoPin, 0, 500);
}
