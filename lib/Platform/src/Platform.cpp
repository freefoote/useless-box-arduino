#include "Platform.h"

#ifdef ARDUINO
    #include <Arduino.h>
    #include <Servo.h>
    #define SUPPRESS_HPP_WARNING
    #include <ServoEasing.hpp>

    static ServoEasing servoInstance;
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

        void servoDetach() {
            servoInstance.detach();
        }

        void servoSmoothMove(uint8_t targetAngle, unsigned long durationMs, uint8_t easingType, bool (*cancellationCheck)()) {
            // Calculate speed in degrees per second from duration
            int angleDifference = (int)targetAngle - (int)currentServoAngle;
            if (angleDifference == 0) {
                return; // Already at target
            }

            // Speed in degrees per second
            uint16_t speedDegPerSec = (uint16_t)((abs(angleDifference) * 1000UL) / durationMs);
            if (speedDegPerSec == 0) speedDegPerSec = 1; // Minimum speed

            // Map easing type to ServoEasing easing function
            uint8_t easingFunc = EASE_LINEAR; // default
            switch (easingType) {
                case 0: easingFunc = EASE_LINEAR; break;
                case 1: easingFunc = EASE_QUADRATIC_IN_OUT; break;
                case 2: easingFunc = EASE_CUBIC_IN_OUT; break;
                case 3: easingFunc = EASE_QUARTIC_IN_OUT; break;
                case 4: easingFunc = EASE_SINE_IN_OUT; break;
                default: easingFunc = EASE_LINEAR; break;
            }

            // Set easing type and move to target angle with specified speed
            servoInstance.setEasingType(easingFunc);
            servoInstance.easeTo(targetAngle, speedDegPerSec);

            // Wait for movement to complete, checking for cancellation
            while (servoInstance.isMoving()) {
                if (cancellationCheck && !cancellationCheck()) {
                    servoInstance.stop();
                    return; // Exit early if cancelled
                }
                delay(10);
            }

            currentServoAngle = targetAngle;
        }
    }
#else
    #include <cmath>
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

        void servoDetach() {
            // Mock: just clear the servo angle
            mockServoAngle = 0;
        }

         void servoSmoothMove(uint8_t targetAngle, unsigned long durationMs, uint8_t easingType, bool (*cancellationCheck)()) {
             unsigned long startTime = mockMillisValue;
             uint8_t startAngle = mockServoAngle;
             int angleDifference = (int)targetAngle - (int)startAngle;

             // Simple easing function implementation for testing
             // Maps easing type to a basic easing curve
             auto applyEasing = [](float progress, uint8_t easingType) -> float {
                 switch (easingType) {
                     case 0: // EASE_LINEAR
                         return progress;
                     case 1: // EASE_QUADRATIC_IN_OUT
                         return progress < 0.5f ? 2.0f * progress * progress : -1.0f + (4.0f - 2.0f * progress) * progress;
                     case 2: // EASE_CUBIC_IN_OUT
                         return progress < 0.5f ? 4.0f * progress * progress * progress : 1.0f + (progress - 1.0f) * (2.0f * (progress - 2.0f) * (progress - 2.0f));
                     case 3: // EASE_QUARTIC_IN_OUT
                         return progress < 0.5f ? 8.0f * progress * progress * progress * progress : 1.0f - 8.0f * (progress - 1.0f) * (progress - 1.0f) * (progress - 1.0f) * (progress - 1.0f);
                     case 4: // EASE_SINE_IN_OUT
                         return (progress < 0.5f) ? 0.5f * (1.0f - cosf(3.14159f * progress)) : 0.5f * (1.0f + cosf(3.14159f * (progress - 1.0f)));
                     default:
                         return progress;
                 }
             };

             while (mockMillisValue - startTime < durationMs) {
                 // Check for cancellation
                 if (cancellationCheck && !cancellationCheck()) {
                     return; // Exit early if cancelled
                 }

                 unsigned long elapsedTime = mockMillisValue - startTime;
                 float progress = (float)elapsedTime / (float)durationMs;
                 float easedProgress = applyEasing(progress, easingType);
                 uint8_t currentAngle = startAngle + (uint8_t)(angleDifference * easedProgress);
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
