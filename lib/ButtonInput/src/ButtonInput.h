#ifndef BUTTON_INPUT_H
#define BUTTON_INPUT_H

#ifdef ARDUINO
    #include <Arduino.h>
#else
    #include <cstdint>
#endif
#include "../../../lib/Platform/src/Platform.h"

/**
 * Handles button input with debouncing
 */
class ButtonInput {
private:
    uint8_t pin;
    uint8_t debounceDelay;
    uint8_t lastState;
    unsigned long lastDebounceTime;
    uint8_t lastReportedPin;

public:
    /**
     * Constructor
     *
     * @param buttonPin The digital pin connected to the button
     * @param debounceMs Debounce delay in milliseconds (default 50ms)
     */
    ButtonInput(uint8_t buttonPin, uint8_t debounceMs = 50);

    /**
     * Initialize the button input
     */
    void begin();

    /**
     * Check if the button has been pressed (rising edge detection)
     *
     * @return true if button was pressed, false otherwise
     */
    bool isPressed();

    /**
     * Get the current state of the button
     *
     * @return true if button is currently pressed, false otherwise
     */
    bool getState() const;

    /**
     * Reset the button press flag to clear the "pressed" state
     * Useful for clearing the flag after a known action completes
     */
    void resetPressFlag();
};

#endif // BUTTON_INPUT_H
