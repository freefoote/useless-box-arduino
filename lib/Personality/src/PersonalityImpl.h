#ifndef PERSONALITY_IMPL_H
#define PERSONALITY_IMPL_H

#include "Personality.h"
#include "../../../lib/Platform/src/Platform.h"

/**
 * Fast personality - quickly flicks the switch off and returns
 */
class FastPersonality : public Personality {
public:
    void execute(uint8_t servoPin, CancellationCheckFn cancellationCheck = nullptr) override;
    const char* getName() const override { return "Fast"; }
    unsigned long getDuration() const override { return 500; }
};

/**
 * Slow personality - deliberately slow and methodical
 */
class SlowPersonality : public Personality {
public:
    void execute(uint8_t servoPin, CancellationCheckFn cancellationCheck = nullptr) override;
    const char* getName() const override { return "Slow"; }
    unsigned long getDuration() const override { return 2000; }
};

/**
 * Jittery personality - nervous, twitchy movements
 */
class JitteryPersonality : public Personality {
public:
    void execute(uint8_t servoPin, CancellationCheckFn cancellationCheck = nullptr) override;
    const char* getName() const override { return "Jittery"; }
    unsigned long getDuration() const override { return 1500; }
};

/**
 * Lazy personality - reluctant, minimal effort
 */
class LazyPersonality : public Personality {
public:
    void execute(uint8_t servoPin, CancellationCheckFn cancellationCheck = nullptr) override;
    const char* getName() const override { return "Lazy"; }
    unsigned long getDuration() const override { return 3000; }
};

#endif // PERSONALITY_IMPL_H
