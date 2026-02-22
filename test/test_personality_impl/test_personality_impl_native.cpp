/**
 * Unit tests for personality implementations (PersonalityImpl.cpp).
 *
 * Each personality is tested across three execution paths:
 *   1. No cancellationCheck (nullptr) — completes normally.
 *   2. Always-continue check (returns true) — completes normally.
 *   3. Always-cancel check (returns false) — triggers reversal path.
 *
 * In all three cases the servo must end at ServoConfig::START_ANGLE because
 * every personality either finishes its forward move and explicitly returns
 * to start, or detects cancellation and reverses back to start.
 *
 * For personalities that contain an explicit Platform::delay() pause
 * (LazyPersonality, InsistentOffPersonality) we also verify that:
 *   - The pause IS included in normal execution (mock time >= PAUSE_DURATION).
 *   - The pause is NOT reached when cancelled (mock time < OFF_DURATION).
 *
 * For JitteryPersonality we verify that all three inter-twitch delays are
 * executed during normal execution but skipped when cancelled early.
 */

#include <unity.h>
#include "../../lib/Platform/src/Platform.h"
#include "../../lib/Personality/src/PersonalityImpl.h"
#include "../../include/config.h"

// ---------------------------------------------------------------------------
// Reusable cancellation-check helpers
// ---------------------------------------------------------------------------

/** Simulates a button that never releases — personality runs to completion. */
static bool alwaysContinue(void) { return true; }

/** Simulates an immediate button release — triggers the reversal path. */
static bool alwaysCancel(void) { return false; }

// ---------------------------------------------------------------------------
// Test fixture
// ---------------------------------------------------------------------------

void setUp(void) {
    Platform::Test::reset();
    // Start the mock servo at the position the hardware would initialise it to
    // so execution paths are as realistic as possible.
    Platform::servoWrite(ServoConfig::START_ANGLE);
}

void tearDown(void) {}

// ===========================================================================
// FastPersonality
// ===========================================================================

void test_fast_personality_name(void) {
    FastPersonality p;
    TEST_ASSERT_EQUAL_STRING("Fast", p.getName());
}

void test_fast_personality_duration(void) {
    FastPersonality p;
    TEST_ASSERT_EQUAL_UINT32(500, p.getDuration());
}

void test_fast_personality_execute_no_cancellation_check(void) {
    FastPersonality p;
    p.execute(8, nullptr);
    TEST_ASSERT_EQUAL_UINT8(ServoConfig::START_ANGLE, Platform::Test::getServoAngle());
}

void test_fast_personality_execute_button_held(void) {
    FastPersonality p;
    p.execute(8, alwaysContinue);
    TEST_ASSERT_EQUAL_UINT8(ServoConfig::START_ANGLE, Platform::Test::getServoAngle());
}

void test_fast_personality_execute_button_released_reverses_to_start(void) {
    FastPersonality p;
    p.execute(8, alwaysCancel);
    TEST_ASSERT_EQUAL_UINT8(ServoConfig::START_ANGLE, Platform::Test::getServoAngle());
}

// ===========================================================================
// SlowPersonality
// ===========================================================================

void test_slow_personality_name(void) {
    SlowPersonality p;
    TEST_ASSERT_EQUAL_STRING("Slow", p.getName());
}

void test_slow_personality_duration(void) {
    SlowPersonality p;
    TEST_ASSERT_EQUAL_UINT32(2000, p.getDuration());
}

void test_slow_personality_execute_no_cancellation_check(void) {
    SlowPersonality p;
    p.execute(8, nullptr);
    TEST_ASSERT_EQUAL_UINT8(ServoConfig::START_ANGLE, Platform::Test::getServoAngle());
}

void test_slow_personality_execute_button_held(void) {
    SlowPersonality p;
    p.execute(8, alwaysContinue);
    TEST_ASSERT_EQUAL_UINT8(ServoConfig::START_ANGLE, Platform::Test::getServoAngle());
}

void test_slow_personality_execute_button_released_reverses_to_start(void) {
    SlowPersonality p;
    p.execute(8, alwaysCancel);
    TEST_ASSERT_EQUAL_UINT8(ServoConfig::START_ANGLE, Platform::Test::getServoAngle());
}

// ===========================================================================
// JitteryPersonality
// ===========================================================================

void test_jittery_personality_name(void) {
    JitteryPersonality p;
    TEST_ASSERT_EQUAL_STRING("Jittery", p.getName());
}

void test_jittery_personality_duration(void) {
    JitteryPersonality p;
    TEST_ASSERT_EQUAL_UINT32(1500, p.getDuration());
}

void test_jittery_personality_execute_no_cancellation_check(void) {
    JitteryPersonality p;
    p.execute(8, nullptr);
    TEST_ASSERT_EQUAL_UINT8(ServoConfig::START_ANGLE, Platform::Test::getServoAngle());
}

void test_jittery_personality_execute_button_held(void) {
    JitteryPersonality p;
    p.execute(8, alwaysContinue);
    TEST_ASSERT_EQUAL_UINT8(ServoConfig::START_ANGLE, Platform::Test::getServoAngle());
}

void test_jittery_personality_execute_button_released_reverses_to_start(void) {
    JitteryPersonality p;
    p.execute(8, alwaysCancel);
    TEST_ASSERT_EQUAL_UINT8(ServoConfig::START_ANGLE, Platform::Test::getServoAngle());
}

/**
 * The jittery personality calls Platform::delay(TWITCH_DELAY) three times
 * during a normal run.  Mock time must therefore advance by at least
 * 3 * TWITCH_DELAY during full execution.
 */
void test_jittery_personality_all_twitch_delays_fire_on_normal_execution(void) {
    JitteryPersonality p;
    p.execute(8, nullptr);
    TEST_ASSERT_TRUE(Platform::millis() >= JitteryPersonalityConfig::TWITCH_DELAY * 3);
}

/**
 * When cancelled immediately (on the very first move) the personality returns
 * before any Platform::delay() call.  Total mock time must be less than the
 * combined duration of the three twitch delays.
 */
void test_jittery_personality_twitch_delays_skipped_when_cancelled(void) {
    JitteryPersonality p;
    p.execute(8, alwaysCancel);
    TEST_ASSERT_TRUE(Platform::millis() < JitteryPersonalityConfig::TWITCH_DELAY * 3);
}

// ===========================================================================
// LazyPersonality
// ===========================================================================

void test_lazy_personality_name(void) {
    LazyPersonality p;
    TEST_ASSERT_EQUAL_STRING("Lazy", p.getName());
}

void test_lazy_personality_duration(void) {
    LazyPersonality p;
    TEST_ASSERT_EQUAL_UINT32(3000, p.getDuration());
}

void test_lazy_personality_execute_no_cancellation_check(void) {
    LazyPersonality p;
    p.execute(8, nullptr);
    TEST_ASSERT_EQUAL_UINT8(ServoConfig::START_ANGLE, Platform::Test::getServoAngle());
}

void test_lazy_personality_execute_button_held(void) {
    LazyPersonality p;
    p.execute(8, alwaysContinue);
    TEST_ASSERT_EQUAL_UINT8(ServoConfig::START_ANGLE, Platform::Test::getServoAngle());
}

void test_lazy_personality_execute_button_released_reverses_to_start(void) {
    LazyPersonality p;
    p.execute(8, alwaysCancel);
    TEST_ASSERT_EQUAL_UINT8(ServoConfig::START_ANGLE, Platform::Test::getServoAngle());
}

/**
 * Normal execution includes the PAUSE_DURATION delay before the return move.
 * Total mock time must be at least PAUSE_DURATION.
 */
void test_lazy_personality_pause_fires_on_normal_execution(void) {
    LazyPersonality p;
    p.execute(8, nullptr);
    TEST_ASSERT_TRUE(Platform::millis() >= LazyPersonalityConfig::PAUSE_DURATION);
}

/**
 * When cancelled, the personality returns before the pause.
 * Total mock time must be less than OFF_DURATION (the cancelled move never
 * fully completes, so elapsed time is only the reversal ON_DURATION).
 */
void test_lazy_personality_pause_skipped_when_cancelled(void) {
    LazyPersonality p;
    p.execute(8, alwaysCancel);
    TEST_ASSERT_TRUE(Platform::millis() < LazyPersonalityConfig::OFF_DURATION);
}

// ===========================================================================
// InsistentOffPersonality
// ===========================================================================

void test_insistent_off_personality_name(void) {
    InsistentOffPersonality p;
    TEST_ASSERT_EQUAL_STRING("InsistentOff", p.getName());
}

void test_insistent_off_personality_duration(void) {
    InsistentOffPersonality p;
    TEST_ASSERT_EQUAL_UINT32(4000, p.getDuration());
}

void test_insistent_off_personality_execute_no_cancellation_check(void) {
    InsistentOffPersonality p;
    p.execute(8, nullptr);
    TEST_ASSERT_EQUAL_UINT8(ServoConfig::START_ANGLE, Platform::Test::getServoAngle());
}

void test_insistent_off_personality_execute_button_held(void) {
    InsistentOffPersonality p;
    p.execute(8, alwaysContinue);
    TEST_ASSERT_EQUAL_UINT8(ServoConfig::START_ANGLE, Platform::Test::getServoAngle());
}

void test_insistent_off_personality_execute_button_released_reverses_to_start(void) {
    InsistentOffPersonality p;
    p.execute(8, alwaysCancel);
    TEST_ASSERT_EQUAL_UINT8(ServoConfig::START_ANGLE, Platform::Test::getServoAngle());
}

/**
 * Normal execution includes a deliberate PAUSE_DURATION hold at the off
 * position ("don't try this again").  Total mock time must be at least
 * PAUSE_DURATION.
 */
void test_insistent_off_personality_pause_fires_on_normal_execution(void) {
    InsistentOffPersonality p;
    p.execute(8, nullptr);
    TEST_ASSERT_TRUE(Platform::millis() >= InsistentOffPersonalityConfig::PAUSE_DURATION);
}

/**
 * When cancelled, the personality returns before reaching the pause.
 * Total mock time must be less than OFF_DURATION.
 */
void test_insistent_off_personality_pause_skipped_when_cancelled(void) {
    InsistentOffPersonality p;
    p.execute(8, alwaysCancel);
    TEST_ASSERT_TRUE(Platform::millis() < InsistentOffPersonalityConfig::OFF_DURATION);
}

// ===========================================================================
// Main
// ===========================================================================

int main(int argc, char **argv) {
    UNITY_BEGIN();

    // --- FastPersonality ---
    RUN_TEST(test_fast_personality_name);
    RUN_TEST(test_fast_personality_duration);
    RUN_TEST(test_fast_personality_execute_no_cancellation_check);
    RUN_TEST(test_fast_personality_execute_button_held);
    RUN_TEST(test_fast_personality_execute_button_released_reverses_to_start);

    // --- SlowPersonality ---
    RUN_TEST(test_slow_personality_name);
    RUN_TEST(test_slow_personality_duration);
    RUN_TEST(test_slow_personality_execute_no_cancellation_check);
    RUN_TEST(test_slow_personality_execute_button_held);
    RUN_TEST(test_slow_personality_execute_button_released_reverses_to_start);

    // --- JitteryPersonality ---
    RUN_TEST(test_jittery_personality_name);
    RUN_TEST(test_jittery_personality_duration);
    RUN_TEST(test_jittery_personality_execute_no_cancellation_check);
    RUN_TEST(test_jittery_personality_execute_button_held);
    RUN_TEST(test_jittery_personality_execute_button_released_reverses_to_start);
    RUN_TEST(test_jittery_personality_all_twitch_delays_fire_on_normal_execution);
    RUN_TEST(test_jittery_personality_twitch_delays_skipped_when_cancelled);

    // --- LazyPersonality ---
    RUN_TEST(test_lazy_personality_name);
    RUN_TEST(test_lazy_personality_duration);
    RUN_TEST(test_lazy_personality_execute_no_cancellation_check);
    RUN_TEST(test_lazy_personality_execute_button_held);
    RUN_TEST(test_lazy_personality_execute_button_released_reverses_to_start);
    RUN_TEST(test_lazy_personality_pause_fires_on_normal_execution);
    RUN_TEST(test_lazy_personality_pause_skipped_when_cancelled);

    // --- InsistentOffPersonality ---
    RUN_TEST(test_insistent_off_personality_name);
    RUN_TEST(test_insistent_off_personality_duration);
    RUN_TEST(test_insistent_off_personality_execute_no_cancellation_check);
    RUN_TEST(test_insistent_off_personality_execute_button_held);
    RUN_TEST(test_insistent_off_personality_execute_button_released_reverses_to_start);
    RUN_TEST(test_insistent_off_personality_pause_fires_on_normal_execution);
    RUN_TEST(test_insistent_off_personality_pause_skipped_when_cancelled);

    return UNITY_END();
}
