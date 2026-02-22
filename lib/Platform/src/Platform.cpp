#include "Platform.h"

#ifdef ARDUINO
    #include <Arduino.h>
    #include <Servo.h>

    static Servo servoInstance;
    static uint8_t currentServoAngle = 0;

    namespace Platform {
        void pwmWrite(uint8_t pin, uint8_t value) {
            analogWrite(pin, value);
        }

        void digitalWrite(uint8_t pin, uint8_t value) {
            ::digitalWrite(pin, value);
        }

        uint8_t digitalRead(uint8_t pin) {
            return ::digitalRead(pin);
        }

        void pinMode(uint8_t pin, uint8_t mode) {
            ::pinMode(pin, mode);
        }

        unsigned long millis() {
            return ::millis();
        }

        void delay(unsigned long ms) {
            ::delay(ms);
        }

        void delayMicroseconds(unsigned int us) {
            ::delayMicroseconds(us);
        }

        void servoInit(uint8_t pin) {
            servoInstance.attach(pin);
            currentServoAngle = 0;
        }

        void servoWrite(uint8_t angle) {
            servoInstance.write(angle);
            currentServoAngle = angle;
        }

        void servoSmoothMove(uint8_t targetAngle, unsigned long durationMs, bool (*cancellationCheck)()) {
            unsigned long startTime = ::millis();
            uint8_t startAngle = currentServoAngle;
            int angleDifference = (int)targetAngle - (int)startAngle;

            while (::millis() - startTime < durationMs) {
                // Check for cancellation
                if (cancellationCheck && !cancellationCheck()) {
                    return; // Exit early if cancelled
                }

                unsigned long elapsedTime = ::millis() - startTime;
                // Linear interpolation: calculate current angle based on elapsed time
                uint8_t currentAngle = startAngle + (angleDifference * elapsedTime) / durationMs;
                servoWrite(currentAngle);

                // Small delay to prevent busy-waiting and allow servo to move
                // ::delay(10);
            }

            // Ensure we reach the exact target angle
            servoWrite(targetAngle);
        }
    }
#else
    // Mock implementations for testing
    namespace Platform {
        static uint8_t mockPinMode[14];
        static uint8_t mockDigitalRead[14];
        static uint8_t mockPwmWrite[14];
        static unsigned long mockMillisValue = 0;
        static uint8_t mockServoAngle = 0;

        void pwmWrite(uint8_t pin, uint8_t value) {
            if (pin < 14) {
                mockPwmWrite[pin] = value;
            }
        }

        void digitalWrite(uint8_t pin, uint8_t value) {
            if (pin < 14) {
                mockDigitalRead[pin] = value;
            }
        }

        uint8_t digitalRead(uint8_t pin) {
            if (pin < 14) {
                return mockDigitalRead[pin];
            }
            return 0;
        }

        void pinMode(uint8_t pin, uint8_t mode) {
            if (pin < 14) {
                mockPinMode[pin] = mode;
            }
        }

        unsigned long millis() {
            return mockMillisValue;
        }

        void delay(unsigned long ms) {
            mockMillisValue += ms;
        }

        void delayMicroseconds(unsigned int us) {
            // No-op for testing
        }

        void servoInit(uint8_t pin) {
            // Mock: just track that servo was initialized on this pin
            if (pin < 14) {
                mockPinMode[pin] = 0xFF; // Mark as servo pin
            }
            mockServoAngle = 0;
        }

        void servoWrite(uint8_t angle) {
            mockServoAngle = angle;
        }

        void servoSmoothMove(uint8_t targetAngle, unsigned long durationMs, bool (*cancellationCheck)()) {
            unsigned long startTime = mockMillisValue;
            uint8_t startAngle = mockServoAngle;
            int angleDifference = (int)targetAngle - (int)startAngle;

            while (mockMillisValue - startTime < durationMs) {
                // Check for cancellation
                if (cancellationCheck && !cancellationCheck()) {
                    return; // Exit early if cancelled
                }

                unsigned long elapsedTime = mockMillisValue - startTime;
                // Linear interpolation: calculate current angle based on elapsed time
                uint8_t currentAngle = startAngle + (angleDifference * elapsedTime) / durationMs;
                servoWrite(currentAngle);

                // Small delay to advance mock time
                delay(10);
            }

            // Ensure we reach the exact target angle
            servoWrite(targetAngle);
        }
    }

    // Test helper functions
    namespace Platform {
        namespace Test {
            void setMillis(unsigned long ms) {
                mockMillisValue = ms;
            }

            void setDigitalRead(uint8_t pin, uint8_t value) {
                if (pin < 14) {
                    mockDigitalRead[pin] = value;
                }
            }

            uint8_t getPinMode(uint8_t pin) {
                if (pin < 14) {
                    return mockPinMode[pin];
                }
                return 0;
            }

            uint8_t getDigitalRead(uint8_t pin) {
                if (pin < 14) {
                    return mockDigitalRead[pin];
                }
                return 0;
            }

            uint8_t getPwmWrite(uint8_t pin) {
                if (pin < 14) {
                    return mockPwmWrite[pin];
                }
                return 0;
            }

            uint8_t getServoAngle() {
                return mockServoAngle;
            }

            void reset() {
                for (int i = 0; i < 14; i++) {
                    mockPinMode[i] = 0;
                    mockDigitalRead[i] = 1; // HIGH
                    mockPwmWrite[i] = 0;
                }
                mockMillisValue = 0;
                mockServoAngle = 0;
            }
        }
    }
#endif
