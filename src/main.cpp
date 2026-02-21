#include <Arduino.h>
#include "../lib/Personality/src/Personality.h"
#include "../lib/Personality/src/PersonalityImpl.h"
#include "../lib/PersonalityManager/src/PersonalityManager.h"
#include "../lib/ButtonInput/src/ButtonInput.h"

// Pin definitions
const uint8_t BUTTON_PIN = 2;
const uint8_t SERVO_PIN = 9;

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

void setup() {
    Serial.begin(9600);
    button.begin();
    pinMode(SERVO_PIN, OUTPUT);

    Serial.println("Useless Box initialized!");
    Serial.print("Starting personality: ");
    Serial.println(personalityManager.getCurrent()->getName());
}

void loop() {
    // Check if button was pressed
    if (button.isPressed()) {
        Serial.print("Button pressed! Executing personality: ");
        Serial.println(personalityManager.getCurrent()->getName());

        // Execute current personality
        personalityManager.executeCurrent(SERVO_PIN);

        // Move to next personality for next activation
        personalityManager.nextPersonality();
        Serial.print("Next personality will be: ");
        Serial.println(personalityManager.getCurrent()->getName());
    }

    delay(10); // Small delay to prevent overwhelming the loop
}
