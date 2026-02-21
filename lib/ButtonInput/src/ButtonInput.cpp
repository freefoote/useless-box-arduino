#include "ButtonInput.h"

ButtonInput::ButtonInput(uint8_t buttonPin, uint8_t debounceMs)
    : pin(buttonPin), debounceDelay(debounceMs), lastState(LOW), lastDebounceTime(0) {
}

void ButtonInput::begin() {
    pinMode(pin, INPUT_PULLUP);
    lastState = digitalRead(pin);
}

bool ButtonInput::isPressed() {
    uint8_t reading = digitalRead(pin);

    // If the button state has changed
    if (reading != lastState) {
        lastDebounceTime = millis();
    }

    // If enough time has passed for debouncing
    if ((millis() - lastDebounceTime) > debounceDelay) {
        // Check for rising edge (button pressed, LOW to HIGH with pullup)
        if (reading == LOW && lastState == HIGH) {
            lastState = reading;
            return true;
        }
        lastState = reading;
    }

    return false;
}

bool ButtonInput::getState() const {
    return digitalRead(pin) == LOW;
}
