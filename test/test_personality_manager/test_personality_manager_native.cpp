#include <unity.h>
#include <cstring>
#include "../../lib/Personality/src/Personality.h"
#include "../../lib/Personality/src/PersonalityImpl.h"
#include "../../lib/PersonalityManager/src/PersonalityManager.h"

// Mock personality for testing
class MockPersonality : public Personality {
public:
    int executeCount = 0;
    uint8_t lastServoPin = 0;

    void execute(uint8_t servoPin) override {
        executeCount++;
        lastServoPin = servoPin;
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
    RUN_TEST(test_personality_names);
    RUN_TEST(test_personality_durations);
    return UNITY_END();
}
