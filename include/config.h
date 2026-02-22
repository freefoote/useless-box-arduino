#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

// ============================================================================
// PIN DEFINITIONS
// ============================================================================

// Button input pin
const uint8_t BUTTON_PIN = 2;

// Servo control pin
const uint8_t SERVO_PIN = 8;

#define IDLE_ANGLE 180
#define SWITCH_OFF_ANGLE 30

// ============================================================================
// PERSONALITY CONFIGURATION - SERVO ANGLES
// ============================================================================

// Global servo configuration
namespace ServoConfig {
    const uint8_t START_ANGLE = IDLE_ANGLE;  // Idle/start position (on position)
}

// Fast Personality - quick flick
namespace FastPersonalityConfig {
    const uint8_t OFF_ANGLE = SWITCH_OFF_ANGLE;           // Angle to move to (off position)
    const unsigned long OFF_DURATION = 500; // Time to move off (ms)
    const unsigned long ON_DURATION = 200;  // Time to return on (ms)
}

// Slow Personality - deliberate and methodical
namespace SlowPersonalityConfig {
    const uint8_t OFF_ANGLE = SWITCH_OFF_ANGLE;            // Angle to move to (off position)
    const unsigned long OFF_DURATION = 1500; // Time to move off (ms)
    const unsigned long ON_DURATION = 500;   // Time to return on (ms)
}

// Jittery Personality - nervous twitches
namespace JitteryPersonalityConfig {
    const uint8_t TWITCH_1_ANGLE = 100;       // First twitch angle
    const unsigned long TWITCH_1_DURATION = 100;
    const unsigned long TWITCH_DELAY = 100;  // Delay between twitches
    const uint8_t TWITCH_2_ANGLE = 60;       // Second twitch angle
    const unsigned long TWITCH_2_DURATION = 150;
    const uint8_t OFF_ANGLE = SWITCH_OFF_ANGLE;            // Final off angle
    const unsigned long OFF_DURATION = 500;  // Time to move off
    const unsigned long ON_DURATION = 200;   // Time to return on
}

// Lazy Personality - reluctant and slow
namespace LazyPersonalityConfig {
    const uint8_t OFF_ANGLE = SWITCH_OFF_ANGLE;            // Angle to move to (off position)
    const unsigned long OFF_DURATION = 2000; // Time to move off (ms)
    const unsigned long PAUSE_DURATION = 500; // Pause before returning
    const unsigned long ON_DURATION = 500;   // Time to return on (ms)
}

// Insistent Off Personality - slowly moves to off, pauses, then retracts
namespace InsistentOffPersonalityConfig {
    const uint8_t OFF_ANGLE = SWITCH_OFF_ANGLE;            // Angle to move to (off position)
    const unsigned long OFF_DURATION = 1500; // Time to move off slowly (ms)
    const unsigned long PAUSE_DURATION = 2000; // Pause at off position (ms) - "don't try this again"
    const unsigned long ON_DURATION = 500;   // Time to return on (ms)
}

#endif // CONFIG_H
