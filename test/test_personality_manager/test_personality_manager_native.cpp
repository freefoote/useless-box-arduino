#include <unity.h>
#include <cstring>
#include "../../lib/Platform/src/Platform.h"
#include "../../lib/Personality/src/Personality.h"
#include "../../lib/Personality/src/PersonalityImpl.h"
#include "../../lib/PersonalityManager/src/PersonalityManager.h"

// Mock personality for testing
class MockPersonality : public Personality {
public:
    int executeCount = 0;
    uint8_t lastServoPin = 0;
    CancellationCheckFn lastCancellationCheck = nullptr;
    bool cancellationCheckCalled = false;

    void execute(uint8_t servoPin, CancellationCheckFn cancellationCheck = nullptr) override {
        executeCount++;
        lastServoPin = servoPin;
        lastCancellationCheck = cancellationCheck;

        // Simulate calling the cancellation check if provided
        if (cancellationCheck) {
            cancellationCheckCalled = true;
            cancellationCheck();
        }
    }

    const char* getName() const override {
        return "Mock";
    }

    unsigned long getDuration() const override {
        return 100;
    }
};

void setUp(void) {
    // Set up before each test
}

void tearDown(void) {
    // Clean up after each test
}

void test_personality_manager_initialization(void) {
    MockPersonality p1, p2, p3;
    Personality* personalities[] = {&p1, &p2, &p3};
    PersonalityManager manager(personalities, 3);

    TEST_ASSERT_EQUAL_UINT8(3, manager.getCount());
    TEST_ASSERT_EQUAL_UINT8(0, manager.getCurrentIndex());
    TEST_ASSERT_EQUAL_PTR(&p1, manager.getCurrent());
}

void test_personality_manager_execute_current(void) {
    MockPersonality p1, p2;
    Personality* personalities[] = {&p1, &p2};
    PersonalityManager manager(personalities, 2);

    manager.executeCurrent(9);
    TEST_ASSERT_EQUAL_INT(1, p1.executeCount);
    TEST_ASSERT_EQUAL_UINT8(9, p1.lastServoPin);
    TEST_ASSERT_EQUAL_INT(0, p2.executeCount);
}

void test_personality_manager_next_personality(void) {
    MockPersonality p1, p2, p3;
    Personality* personalities[] = {&p1, &p2, &p3};
    PersonalityManager manager(personalities, 3);

    TEST_ASSERT_EQUAL_UINT8(0, manager.getCurrentIndex());
    manager.nextPersonality();
    TEST_ASSERT_EQUAL_UINT8(1, manager.getCurrentIndex());
    TEST_ASSERT_EQUAL_PTR(&p2, manager.getCurrent());

    manager.nextPersonality();
    TEST_ASSERT_EQUAL_UINT8(2, manager.getCurrentIndex());
    TEST_ASSERT_EQUAL_PTR(&p3, manager.getCurrent());

    manager.nextPersonality();
    TEST_ASSERT_EQUAL_UINT8(0, manager.getCurrentIndex());
    TEST_ASSERT_EQUAL_PTR(&p1, manager.getCurrent());
}

void test_personality_manager_cycle(void) {
    MockPersonality p1, p2;
    Personality* personalities[] = {&p1, &p2};
    PersonalityManager manager(personalities, 2);

    // Execute and cycle through personalities
    manager.executeCurrent(5);
    TEST_ASSERT_EQUAL_INT(1, p1.executeCount);

    manager.nextPersonality();
    manager.executeCurrent(5);
    TEST_ASSERT_EQUAL_INT(1, p2.executeCount);

    manager.nextPersonality();
    manager.executeCurrent(5);
    TEST_ASSERT_EQUAL_INT(2, p1.executeCount);
}

void test_personality_manager_execute_with_cancellation_check(void) {
    MockPersonality p1;
    Personality* personalities[] = {&p1};
    PersonalityManager manager(personalities, 1);

    // Create a simple cancellation check function
    bool buttonPressed = true;
    auto checkButton = [](void) -> bool {
        return true; // Button still pressed
    };

    manager.executeCurrent(9, checkButton);
    TEST_ASSERT_EQUAL_INT(1, p1.executeCount);
    TEST_ASSERT_EQUAL_UINT8(9, p1.lastServoPin);
    TEST_ASSERT_TRUE(p1.cancellationCheckCalled);
    TEST_ASSERT_NOT_NULL(p1.lastCancellationCheck);
}

// Global flag for testing callback invocation
static bool g_testCallbackInvoked = false;

static bool testCancellationCallback(void) {
    g_testCallbackInvoked = true;
    return true;
}

void test_personality_execute_with_cancellation_callback(void) {
    // Test that cancellation callback is properly passed through
    MockPersonality mock;
    g_testCallbackInvoked = false;

    mock.execute(5, testCancellationCallback);
    TEST_ASSERT_EQUAL_INT(1, mock.executeCount);
    TEST_ASSERT_TRUE(g_testCallbackInvoked);
}

void test_personality_names(void) {
    FastPersonality fast;
    SlowPersonality slow;
    JitteryPersonality jittery;
    LazyPersonality lazy;

    TEST_ASSERT_EQUAL_STRING("Fast", fast.getName());
    TEST_ASSERT_EQUAL_STRING("Slow", slow.getName());
    TEST_ASSERT_EQUAL_STRING("Jittery", jittery.getName());
    TEST_ASSERT_EQUAL_STRING("Lazy", lazy.getName());
}

void test_personality_durations(void) {
    FastPersonality fast;
    SlowPersonality slow;
    JitteryPersonality jittery;
    LazyPersonality lazy;

    TEST_ASSERT_EQUAL_UINT32(500, fast.getDuration());
    TEST_ASSERT_EQUAL_UINT32(2000, slow.getDuration());
    TEST_ASSERT_EQUAL_UINT32(1500, jittery.getDuration());
    TEST_ASSERT_EQUAL_UINT32(3000, lazy.getDuration());
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_personality_manager_initialization);
    RUN_TEST(test_personality_manager_execute_current);
    RUN_TEST(test_personality_manager_next_personality);
    RUN_TEST(test_personality_manager_cycle);
    RUN_TEST(test_personality_manager_execute_with_cancellation_check);
    RUN_TEST(test_personality_execute_with_cancellation_callback);
    RUN_TEST(test_personality_names);
    RUN_TEST(test_personality_durations);
    return UNITY_END();
}
