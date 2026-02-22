#include "PersonalityImpl.h"
#include "../../../include/config.h"

// Servo control helper functions

// Send a single PWM pulse to the servo for a given angle
void sendServoPulse(uint8_t pin, uint8_t angle) {
    // Map angle (0-180) to PWM value (1000-2000 microseconds for SG90)
    unsigned long pulseWidth = 1000 + (angle * 1000 / 180);
    Platform::digitalWrite(pin, 1); // HIGH
    Platform::delayMicroseconds(pulseWidth);
    Platform::digitalWrite(pin, 0); // LOW
    Platform::delayMicroseconds(20000 - pulseWidth);
}

// Move servo to angle over a specified duration with optional cancellation check
static void moveServoToAngle(uint8_t pin, uint8_t angle, unsigned long duration, CancellationCheckFn cancellationCheck = nullptr) {
    unsigned long startTime = Platform::millis();

    while (Platform::millis() - startTime < duration) {
        // Check for cancellation
        if (cancellationCheck && !cancellationCheck()) {
            return; // Exit early if button released
        }

        sendServoPulse(pin, angle);
    }
}

// Fast personality - quick flick
void FastPersonality::execute(uint8_t servoPin, CancellationCheckFn cancellationCheck) {
    // Move to off position quickly
    moveServoToAngle(servoPin, FastPersonalityConfig::OFF_ANGLE, FastPersonalityConfig::OFF_DURATION, cancellationCheck);

    // Check if we were cancelled mid-execution
    if (cancellationCheck && !cancellationCheck()) {
        // Reverse: return to start position
        moveServoToAngle(servoPin, ServoConfig::START_ANGLE, FastPersonalityConfig::ON_DURATION, nullptr);
        return;
    }

    // Return to start position
    moveServoToAngle(servoPin, ServoConfig::START_ANGLE, FastPersonalityConfig::ON_DURATION, cancellationCheck);
}

// Slow personality - deliberate and methodical
void SlowPersonality::execute(uint8_t servoPin, CancellationCheckFn cancellationCheck) {
    // Slowly move to off position
    moveServoToAngle(servoPin, SlowPersonalityConfig::OFF_ANGLE, SlowPersonalityConfig::OFF_DURATION, cancellationCheck);

    // Check if we were cancelled mid-execution
    if (cancellationCheck && !cancellationCheck()) {
        // Reverse: slowly return to start position
        moveServoToAngle(servoPin, ServoConfig::START_ANGLE, SlowPersonalityConfig::ON_DURATION, nullptr);
        return;
    }

    // Slowly return to start position
    moveServoToAngle(servoPin, ServoConfig::START_ANGLE, SlowPersonalityConfig::ON_DURATION, cancellationCheck);
}

// Jittery personality - nervous twitches
void JitteryPersonality::execute(uint8_t servoPin, CancellationCheckFn cancellationCheck) {
    // Quick twitch up
    moveServoToAngle(servoPin, JitteryPersonalityConfig::TWITCH_1_ANGLE, JitteryPersonalityConfig::TWITCH_1_DURATION, cancellationCheck);
    if (cancellationCheck && !cancellationCheck()) {
        moveServoToAngle(servoPin, ServoConfig::START_ANGLE, JitteryPersonalityConfig::TWITCH_1_DURATION, nullptr);
        return;
    }

    Platform::delay(JitteryPersonalityConfig::TWITCH_DELAY);

    // Twitch back
    moveServoToAngle(servoPin, ServoConfig::START_ANGLE, JitteryPersonalityConfig::TWITCH_1_DURATION, cancellationCheck);
    if (cancellationCheck && !cancellationCheck()) {
        moveServoToAngle(servoPin, ServoConfig::START_ANGLE, JitteryPersonalityConfig::TWITCH_1_DURATION, nullptr);
        return;
    }

    Platform::delay(JitteryPersonalityConfig::TWITCH_DELAY);

    // Another twitch
    moveServoToAngle(servoPin, JitteryPersonalityConfig::TWITCH_2_ANGLE, JitteryPersonalityConfig::TWITCH_2_DURATION, cancellationCheck);
    if (cancellationCheck && !cancellationCheck()) {
        moveServoToAngle(servoPin, ServoConfig::START_ANGLE, JitteryPersonalityConfig::TWITCH_2_DURATION, nullptr);
        return;
    }

    Platform::delay(JitteryPersonalityConfig::TWITCH_DELAY);

    // Final movement to off position
    moveServoToAngle(servoPin, JitteryPersonalityConfig::OFF_ANGLE, JitteryPersonalityConfig::OFF_DURATION, cancellationCheck);
    if (cancellationCheck && !cancellationCheck()) {
        moveServoToAngle(servoPin, ServoConfig::START_ANGLE, JitteryPersonalityConfig::OFF_DURATION, nullptr);
        return;
    }

    // Return
    moveServoToAngle(servoPin, ServoConfig::START_ANGLE, JitteryPersonalityConfig::ON_DURATION, cancellationCheck);
}

// Lazy personality - reluctant and slow
void LazyPersonality::execute(uint8_t servoPin, CancellationCheckFn cancellationCheck) {
    // Slow, lazy movement to off position
    moveServoToAngle(servoPin, LazyPersonalityConfig::OFF_ANGLE, LazyPersonalityConfig::OFF_DURATION, cancellationCheck);

    // Check if we were cancelled mid-execution
    if (cancellationCheck && !cancellationCheck()) {
        // Reverse: slow return to start position
        moveServoToAngle(servoPin, ServoConfig::START_ANGLE, LazyPersonalityConfig::ON_DURATION, nullptr);
        return;
    }

    Platform::delay(LazyPersonalityConfig::PAUSE_DURATION);

    // Slow return to start position
    moveServoToAngle(servoPin, ServoConfig::START_ANGLE, LazyPersonalityConfig::ON_DURATION, cancellationCheck);
}
