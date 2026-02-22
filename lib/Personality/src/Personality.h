#ifndef PERSONALITY_H
#define PERSONALITY_H

#ifdef ARDUINO
    #include <Arduino.h>
#else
    #include <cstdint>
#endif

/**
 * Callback function type for checking if execution should be cancelled
 * Returns true if the button is still pressed (continue), false if released (cancel)
 */
typedef bool (*CancellationCheckFn)();

/**
 * Base class for useless box personalities.
 * Each personality defines a unique behavior pattern for the servo
 * when the button is activated.
 */
class Personality {
public:
    virtual ~Personality() = default;

    /**
     * Execute the personality behavior.
     * This method should control the servo through the provided pin
     * to perform the personality's unique action.
     *
     * @param servoPin The PWM pin connected to the servo
     * @param cancellationCheck Optional callback to check if execution should be cancelled.
     *                          If provided and returns false, execution should reverse.
     */
    virtual void execute(uint8_t servoPin, CancellationCheckFn cancellationCheck = nullptr) = 0;

    /**
     * Get the name of this personality for debugging/logging.
     *
     * @return A string identifier for this personality
     */
    virtual const char* getName() const = 0;

    /**
     * Get the duration in milliseconds that this personality takes to complete.
     * Used for timing and sequencing.
     *
     * @return Duration in milliseconds
     */
    virtual unsigned long getDuration() const = 0;
};

#endif // PERSONALITY_H
