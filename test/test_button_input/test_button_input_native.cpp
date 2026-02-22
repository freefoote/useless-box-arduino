#include <unity.h>
#include "../../lib/Platform/src/Platform.h"
#include "../../lib/ButtonInput/src/ButtonInput.h"

void setUp(void) {
    // Reset mock state
    Platform::Test::reset();
}

void tearDown(void) {
}

void test_button_input_initialization(void) {
    ButtonInput button(2);
    button.begin();

    // Verify pin mode was set to INPUT_PULLUP (2)
    TEST_ASSERT_EQUAL_UINT8(2, Platform::Test::getPinMode(2));
}

void test_button_input_press_detection(void) {
    ButtonInput button(2);
    button.begin();

    // Simulate button press (LOW state = 0)
    Platform::Test::setDigitalRead(2, 0);
    Platform::Test::setMillis(0);

    // First read should detect the change but not trigger press yet
    bool pressed = button.isPressed();
    TEST_ASSERT_FALSE(pressed);

    // After debounce delay, should detect press
    Platform::Test::setMillis(60); // 0ms + 60ms > 50ms debounce
    pressed = button.isPressed();
    TEST_ASSERT_TRUE(pressed);

    // Second call while still pressed should return false (already reported)
    pressed = button.isPressed();
    TEST_ASSERT_FALSE(pressed);

    // Release button
    Platform::Test::setDigitalRead(2, 1);
    Platform::Test::setMillis(120);
    pressed = button.isPressed();
    TEST_ASSERT_FALSE(pressed);
}

void test_button_input_debounce(void) {
    ButtonInput button(2, 50);
    button.begin();

    // Simulate noise (quick transitions)
    Platform::Test::setDigitalRead(2, 0);
    Platform::Test::setMillis(10);
    bool pressed1 = button.isPressed();

    Platform::Test::setDigitalRead(2, 1);
    Platform::Test::setMillis(20);
    bool pressed2 = button.isPressed();

    // Should not detect press due to debounce
    TEST_ASSERT_FALSE(pressed1);
    TEST_ASSERT_FALSE(pressed2);
}

void test_button_input_get_state(void) {
    ButtonInput button(2);
    button.begin();

    // Button not pressed (HIGH = 1)
    Platform::Test::setDigitalRead(2, 1);
    TEST_ASSERT_FALSE(button.getState());

    // Button pressed (LOW = 0)
    Platform::Test::setDigitalRead(2, 0);
    TEST_ASSERT_TRUE(button.getState());
}

void test_button_input_reset_press_flag(void) {
    ButtonInput button(2);
    button.begin();

    // Simulate button press
    Platform::Test::setDigitalRead(2, 0);
    Platform::Test::setMillis(0);

    // First read detects change but doesn't trigger press yet
    bool pressed = button.isPressed();
    TEST_ASSERT_FALSE(pressed);

    // After debounce, should detect press
    Platform::Test::setMillis(60);
    pressed = button.isPressed();
    TEST_ASSERT_TRUE(pressed);

    // Second call while still pressed should return false (already reported)
    pressed = button.isPressed();
    TEST_ASSERT_FALSE(pressed);

    // Reset the press flag
    button.resetPressFlag();

    // Now even though button is still physically pressed, the flag is cleared
    // so the next isPressed() call should detect it as a new press
    pressed = button.isPressed();
    TEST_ASSERT_TRUE(pressed);

    // Second call after reset should return false again
    pressed = button.isPressed();
    TEST_ASSERT_FALSE(pressed);
}

void test_button_input_race_condition_scenario(void) {
    // This test simulates the race condition fix:
    // 1. Button is pressed and detected
    // 2. Action completes with a delay
    // 3. During the delay, user presses button again
    // 4. After delay, flag is reset
    // 5. Next loop iteration should detect the new press

    ButtonInput button(2);
    button.begin();

    // Initial press
    Platform::Test::setDigitalRead(2, 0);
    Platform::Test::setMillis(0);
    bool pressed = button.isPressed();
    TEST_ASSERT_FALSE(pressed);

    Platform::Test::setMillis(60);
    pressed = button.isPressed();
    TEST_ASSERT_TRUE(pressed);

    // Simulate action execution and delay
    Platform::Test::setMillis(160); // 100ms delay

    // User presses button during the delay (button still LOW)
    // isPressed() returns false because flag is already set
    pressed = button.isPressed();
    TEST_ASSERT_FALSE(pressed);

    // After delay, reset the flag (as done in main.cpp)
    button.resetPressFlag();

    // Now the next isPressed() call should detect the press that occurred during delay
    pressed = button.isPressed();
    TEST_ASSERT_TRUE(pressed);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_button_input_initialization);
    RUN_TEST(test_button_input_press_detection);
    RUN_TEST(test_button_input_debounce);
    RUN_TEST(test_button_input_get_state);
    RUN_TEST(test_button_input_reset_press_flag);
    RUN_TEST(test_button_input_race_condition_scenario);
    return UNITY_END();
}
