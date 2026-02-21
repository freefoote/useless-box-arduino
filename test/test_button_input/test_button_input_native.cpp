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

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_button_input_initialization);
    RUN_TEST(test_button_input_press_detection);
    RUN_TEST(test_button_input_debounce);
    RUN_TEST(test_button_input_get_state);
    return UNITY_END();
}
