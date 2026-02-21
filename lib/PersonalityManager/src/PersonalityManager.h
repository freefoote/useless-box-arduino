#ifndef PERSONALITY_MANAGER_H
#define PERSONALITY_MANAGER_H

#include <Arduino.h>
#include "../../../lib/Personality/src/Personality.h"

/**
 * Manages a collection of personalities and cycles through them.
 * Keeps track of the current personality and provides methods to
 * execute and advance to the next personality.
 */
class PersonalityManager {
private:
    Personality** personalities;
    uint8_t personalityCount;
    uint8_t currentIndex;

public:
    /**
     * Constructor
     *
     * @param personalityArray Array of Personality pointers
     * @param count Number of personalities in the array
     */
    PersonalityManager(Personality** personalityArray, uint8_t count);

    /**
     * Execute the current personality
     *
     * @param servoPin The PWM pin connected to the servo
     */
    void executeCurrent(uint8_t servoPin);

    /**
     * Advance to the next personality in the cycle
     */
    void nextPersonality();

    /**
     * Get the current personality
     *
     * @return Pointer to the current Personality
     */
    Personality* getCurrent() const;

    /**
     * Get the index of the current personality
     *
     * @return Current personality index
     */
    uint8_t getCurrentIndex() const { return currentIndex; }

    /**
     * Get the total number of personalities
     *
     * @return Number of personalities
     */
    uint8_t getCount() const { return personalityCount; }
};

#endif // PERSONALITY_MANAGER_H
