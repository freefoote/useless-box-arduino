#include <unity.h>
#include "../../lib/ButtonInput/src/ButtonInput.h"

// Mock Arduino functions for testing
static uint8_t mockPinMode[14];
static uint8_t mockDigitalRead[14];
static unsigned long mockMillis = 0;

// Override Arduino functions
void pinMode(uint8_t pin, uint8_t mode) {
    if (pin < 14) {
        mockPinMode[pin] = mode;
    }
}

int digitalRead(uint8_t pin) {
    if (pin < 14) {
        return mockDigitalRead[pin];
    }
    return LOW;
}

unsigned long millis(void) {
    return mockMillis;
}

void delay(unsigned long ms) {
    mockMillis += ms;
}

void delayMicroseconds(unsigned int us) {
    // No-op for testing
}

void digitalWrite(uint8_t pin, uint8_t val) {
    // No-op for testing
}

void setUp(void) {
    // Reset mock state
    for (int i = 0; i < 14; i++) {
        mockPinMode[i] = 0;
        mockDigitalRead[i] = HIGH;
    }
    mockMillis = 0;
}

void tearDown(void) {
}

void test_button_input_initialization(void) {
    ButtonInput button(2);
    button.begin();

    // Verify pin mode was set to INPUT_PULLUP
    TEST_ASSERT_EQUAL_UINT8(INPUT_PULLUP, mockPinMode[2]);
}

void test_button_input_press_detection(void) {
    ButtonInput button(2);
    button.begin();

    // Simulate button press (LOW state)
    mockDigitalRead[2] = LOW;
    mockMillis = 100;

    // First read should detect the change
    bool pressed = button.isPressed();

    // After debounce delay, should detect press
    mockMillis = 160; // 100ms + 60ms > 50ms debounce
    pressed = button.isPressed();

    TEST_ASSERT_TRUE(pressed);
}

void test_button_input_debounce(void) {
    ButtonInput button(2, 50);
    button.begin();

    // Simulate noise (quick transitions)
    mockDigitalRead[2] = LOW;
    mockMillis = 10;
    bool pressed1 = button.isPressed();

    mockDigitalRead[2] = HIGH;
    mockMillis = 20;
    bool pressed2 = button.isPressed();

    // Should not detect press due to debounce
    TEST_ASSERT_FALSE(pressed1);
    TEST_ASSERT_FALSE(pressed2);
}

void test_button_input_get_state(void) {
    ButtonInput button(2);
    button.begin();

    // Button not pressed
    mockDigitalRead[2] = HIGH;
    TEST_ASSERT_FALSE(button.getState());

    // Button pressed
    mockDigitalRead[2] = LOW;
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
