#include "PersonalityManager.h"

PersonalityManager::PersonalityManager(Personality** personalityArray, uint8_t count)
    : personalities(personalityArray), personalityCount(count), currentIndex(0) {
}

void PersonalityManager::executeCurrent(uint8_t servoPin, CancellationCheckFn cancellationCheck) {
    if (personalityCount > 0 && personalities[currentIndex] != nullptr) {
        personalities[currentIndex]->execute(servoPin, cancellationCheck);
    }
}

void PersonalityManager::nextPersonality() {
    if (personalityCount > 0) {
        currentIndex = (currentIndex + 1) % personalityCount;
    }
}

Personality* PersonalityManager::getCurrent() const {
    if (personalityCount > 0) {
        return personalities[currentIndex];
    }
    return nullptr;
}
