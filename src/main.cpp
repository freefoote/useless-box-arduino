#include <Arduino.h>
#include "../include/config.h"
#include "../lib/Platform/src/Platform.h"
#include "../lib/Personality/src/Personality.h"
#include "../lib/Personality/src/PersonalityImpl.h"
#include "../lib/PersonalityManager/src/PersonalityManager.h"
#include "../lib/ButtonInput/src/ButtonInput.h"

// Forward declaration of servo pulse function from PersonalityImpl
extern void sendServoPulse(uint8_t pin, uint8_t angle);

// Create personality instances
FastPersonality fastPersonality;
SlowPersonality slowPersonality;
JitteryPersonality jitteryPersonality;
LazyPersonality lazyPersonality;

// Create personality array
Personality* personalities[] = {
    &fastPersonality,
    &slowPersonality,
    &jitteryPersonality,
    &lazyPersonality
};

// Create manager
PersonalityManager personalityManager(personalities, 4);

// Create button input
ButtonInput button(BUTTON_PIN);

// Cancellation check callback - returns true if button is still pressed
bool checkButtonStillPressed() {
    return button.getState();
}

void setup() {
    Serial.begin(9600);
    button.begin();
    pinMode(SERVO_PIN, OUTPUT);

    // Initialize servo to idle position
    sendServoPulse(SERVO_PIN, ServoConfig::START_ANGLE);
    Platform::delay(100); // Brief pause to ensure servo reaches position

    Serial.println("Useless Box initialized!");
    Serial.print("Starting personality: ");
    Serial.println(personalityManager.getCurrent()->getName());
}

void loop() {
    // Check if button was pressed
    if (button.isPressed()) {
        Serial.print("Button pressed! Executing personality: ");
        Serial.println(personalityManager.getCurrent()->getName());

        // Execute current personality with cancellation check
        // If button is released during execution, the personality will reverse
        personalityManager.executeCurrent(SERVO_PIN, checkButtonStillPressed);

        // Move to next personality for next activation
        personalityManager.nextPersonality();
        Serial.print("Next personality will be: ");
        Serial.println(personalityManager.getCurrent()->getName());
    }

    delay(10); // Small delay to prevent overwhelming the loop
}
