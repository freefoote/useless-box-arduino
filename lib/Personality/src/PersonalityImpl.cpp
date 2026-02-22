#include "PersonalityImpl.h"

// Servo control helper functions
static void moveServoToAngle(uint8_t pin, uint8_t angle, unsigned long duration, CancellationCheckFn cancellationCheck = nullptr) {
    // Map angle (0-180) to PWM value (1000-2000 microseconds for SG90)
    unsigned long pulseWidth = 1000 + (angle * 1000 / 180);
    unsigned long startTime = Platform::millis();

    while (Platform::millis() - startTime < duration) {
        // Check for cancellation
        if (cancellationCheck && !cancellationCheck()) {
            return; // Exit early if button released
        }

        Platform::digitalWrite(pin, 1); // HIGH
        Platform::delayMicroseconds(pulseWidth);
        Platform::digitalWrite(pin, 0); // LOW
        Platform::delayMicroseconds(20000 - pulseWidth);
    }
}

// Fast personality - quick flick
void FastPersonality::execute(uint8_t servoPin, CancellationCheckFn cancellationCheck) {
    // Move to 90 degrees (off position) quickly
    moveServoToAngle(servoPin, 90, 300, cancellationCheck);

    // Check if we were cancelled mid-execution
    if (cancellationCheck && !cancellationCheck()) {
        // Reverse: return to 0 degrees (on position)
        moveServoToAngle(servoPin, 0, 200, nullptr);
        return;
    }

    // Return to 0 degrees (on position)
    moveServoToAngle(servoPin, 0, 200, cancellationCheck);
}

// Slow personality - deliberate and methodical
void SlowPersonality::execute(uint8_t servoPin, CancellationCheckFn cancellationCheck) {
    // Slowly move to 90 degrees
    moveServoToAngle(servoPin, 90, 1500, cancellationCheck);

    // Check if we were cancelled mid-execution
    if (cancellationCheck && !cancellationCheck()) {
        // Reverse: slowly return to 0 degrees
        moveServoToAngle(servoPin, 0, 500, nullptr);
        return;
    }

    // Slowly return to 0 degrees
    moveServoToAngle(servoPin, 0, 500, cancellationCheck);
}

// Jittery personality - nervous twitches
void JitteryPersonality::execute(uint8_t servoPin, CancellationCheckFn cancellationCheck) {
    // Quick twitch up
    moveServoToAngle(servoPin, 45, 100, cancellationCheck);
    if (cancellationCheck && !cancellationCheck()) {
        moveServoToAngle(servoPin, 0, 100, nullptr);
        return;
    }

    Platform::delay(100);

    // Twitch back
    moveServoToAngle(servoPin, 0, 100, cancellationCheck);
    if (cancellationCheck && !cancellationCheck()) {
        moveServoToAngle(servoPin, 0, 100, nullptr);
        return;
    }

    Platform::delay(100);

    // Another twitch
    moveServoToAngle(servoPin, 60, 150, cancellationCheck);
    if (cancellationCheck && !cancellationCheck()) {
        moveServoToAngle(servoPin, 0, 150, nullptr);
        return;
    }

    Platform::delay(100);

    // Final movement to off position
    moveServoToAngle(servoPin, 90, 300, cancellationCheck);
    if (cancellationCheck && !cancellationCheck()) {
        moveServoToAngle(servoPin, 0, 300, nullptr);
        return;
    }

    // Return
    moveServoToAngle(servoPin, 0, 200, cancellationCheck);
}

// Lazy personality - reluctant and slow
void LazyPersonality::execute(uint8_t servoPin, CancellationCheckFn cancellationCheck) {
    // Slow, lazy movement to off position
    moveServoToAngle(servoPin, 90, 2000, cancellationCheck);

    // Check if we were cancelled mid-execution
    if (cancellationCheck && !cancellationCheck()) {
        // Reverse: slow return
        moveServoToAngle(servoPin, 0, 500, nullptr);
        return;
    }

    Platform::delay(500);

    // Slow return
    moveServoToAngle(servoPin, 0, 500, cancellationCheck);
}
