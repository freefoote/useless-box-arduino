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

    // Test helper namespace
    namespace Test {
        void setMillis(unsigned long ms);
        void setDigitalRead(uint8_t pin, uint8_t value);
        uint8_t getPinMode(uint8_t pin);
        uint8_t getDigitalRead(uint8_t pin);
        uint8_t getPwmWrite(uint8_t pin);
        void reset();
    }
}

#endif // PLATFORM_H
