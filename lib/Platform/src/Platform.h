#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef ARDUINO
    #include <Arduino.h>
#else
    #include <cstdint>
#endif

/**
 * Platform abstraction layer for hardware-specific operations.
 * This allows the core logic to be tested without Arduino.h
 */
namespace Platform {
    /**
     * Write a PWM value to a pin
     *
     * @param pin The pin number
     * @param value PWM value (0-255)
     */
    void pwmWrite(uint8_t pin, uint8_t value);

    /**
     * Set a pin to HIGH or LOW
     *
     * @param pin The pin number
     * @param value HIGH (1) or LOW (0)
     */
    void digitalWrite(uint8_t pin, uint8_t value);

    /**
     * Read the digital state of a pin
     *
     * @param pin The pin number
     * @return HIGH (1) or LOW (0)
     */
    uint8_t digitalRead(uint8_t pin);

    /**
     * Set pin mode
     *
     * @param pin The pin number
     * @param mode INPUT, OUTPUT, or INPUT_PULLUP
     */
    void pinMode(uint8_t pin, uint8_t mode);

    /**
     * Get current time in milliseconds
     *
     * @return Current time in milliseconds
     */
    unsigned long millis();

    /**
     * Delay for specified milliseconds
     *
     * @param ms Milliseconds to delay
     */
    void delay(unsigned long ms);

    /**
     * Delay for specified microseconds
     *
     * @param us Microseconds to delay
     */
    void delayMicroseconds(unsigned int us);

    /**
     * Initialize servo on a pin
     *
     * @param pin The pin number to attach the servo to
     */
    void servoInit(uint8_t pin);

    /**
     * Write servo angle (0-180 degrees)
     *
     * @param angle The angle to move the servo to (0-180)
     */
    void servoWrite(uint8_t angle);

     /**
      * Move servo smoothly to a target angle over a specified duration with easing
      *
      * @param targetAngle The target angle (0-180)
      * @param durationMs The duration in milliseconds to reach the target angle
      * @param easingType The easing function to use (0=linear, 1=quadratic, 2=cubic, etc.)
      * @param cancellationCheck Optional callback to check for cancellation (returns true if should continue)
      */
     void servoSmoothMove(uint8_t targetAngle, unsigned long durationMs, uint8_t easingType = 0, bool (*cancellationCheck)() = nullptr);

    // Test helper namespace
    namespace Test {
        void setMillis(unsigned long ms);
        void setDigitalRead(uint8_t pin, uint8_t value);
        uint8_t getPinMode(uint8_t pin);
        uint8_t getDigitalRead(uint8_t pin);
        uint8_t getPwmWrite(uint8_t pin);
        uint8_t getServoAngle();
        void reset();
    }
}

#endif // PLATFORM_H
