#include "PersonalityManager.h"

PersonalityManager::PersonalityManager(Personality** personalityArray, uint8_t count)
    : personalities(personalityArray), personalityCount(count), currentIndex(0) {
}

void PersonalityManager::executeCurrent(uint8_t servoPin) {
    if (personalityCount > 0 && personalities[currentIndex] != nullptr) {
        personalities[currentIndex]->execute(servoPin);
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
