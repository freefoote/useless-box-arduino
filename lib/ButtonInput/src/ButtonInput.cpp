#include "ButtonInput.h"

ButtonInput::ButtonInput(uint8_t buttonPin, uint8_t debounceMs)
    : pin(buttonPin), debounceDelay(debounceMs), lastState(1), lastDebounceTime(0), lastReportedPin(255) {
}

void ButtonInput::begin() {
    Platform::pinMode(pin, 2); // INPUT_PULLUP = 2
    lastState = Platform::digitalRead(pin);
}

bool ButtonInput::isPressed() {
    uint8_t reading = Platform::digitalRead(pin);

    // If the button state has changed
    if (reading != lastState) {
        lastDebounceTime = Platform::millis();
        lastState = reading;
        return false; // Don't report press until debounce is complete
    }

    // If enough time has passed for debouncing
    if ((Platform::millis() - lastDebounceTime) > debounceDelay) {
        // Check for falling edge (button pressed, HIGH to LOW with pullup)
        if (reading == 0 && lastState == 0) { // Button is held LOW
            // Return true only once per press
            if (lastReportedPin != pin) {
                lastReportedPin = pin;
                return true;
            }
        } else if (reading == 1) {
            // Button released, reset for next press
            lastReportedPin = 255;
        }
    }

    return false;
}

bool ButtonInput::getState() const {
    return Platform::digitalRead(pin) == 0; // LOW = 0
}
