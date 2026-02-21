# Useless Box - Testing Guide

## Overview

This project implements a "useless box" with multiple personalities that cycle through different behaviors when the button is activated. The system is designed with testability in mind, allowing unit tests to run locally before deployment to the Arduino UNO.

## Project Structure

### Libraries

#### [`lib/Personality/`](lib/Personality/)

Base personality system with abstract interface and concrete implementations:

- [`Personality.h`](lib/Personality/src/Personality.h) - Abstract base class defining the personality interface
- [`PersonalityImpl.h`](lib/Personality/src/PersonalityImpl.h) - Concrete personality implementations
- [`PersonalityImpl.cpp`](lib/Personality/src/PersonalityImpl.cpp) - Personality behavior implementations

**Personalities:**

- **Fast** - Quick flick of the switch (500ms)
- **Slow** - Deliberate, methodical movement (2000ms)
- **Jittery** - Nervous, twitchy movements (1500ms)
- **Lazy** - Reluctant, minimal effort (3000ms)

#### [`lib/PersonalityManager/`](lib/PersonalityManager/)

Manages personality cycling:

- [`PersonalityManager.h`](lib/PersonalityManager/src/PersonalityManager.h) - Manager interface
- [`PersonalityManager.cpp`](lib/PersonalityManager/src/PersonalityManager.cpp) - Manager implementation

#### [`lib/ButtonInput/`](lib/ButtonInput/)

Handles button input with debouncing:

- [`ButtonInput.h`](lib/ButtonInput/src/ButtonInput.h) - Button interface
- [`ButtonInput.cpp`](lib/ButtonInput/src/ButtonInput.cpp) - Button implementation

### Main Application

[`src/main.cpp`](src/main.cpp) - Main application that ties everything together

### Tests

#### [`test/test_personality_manager/`](test/test_personality_manager/)

Unit tests for the personality manager system:

- [`test_personality_manager_native.cpp`](test/test_personality_manager/test_personality_manager_native.cpp)

Tests include:

- Personality manager initialization
- Current personality execution
- Personality cycling
- Personality metadata (names, durations)

#### [`test/test_button_input/`](test/test_button_input/)

Unit tests for button input handling:

- [`test_button_input_native.cpp`](test/test_button_input/test_button_input_native.cpp)

Tests include:

- Button initialization
- Press detection
- Debounce behavior
- State reading

## Running Tests

### Prerequisites

You need a C/C++ compiler installed on your system. On Windows, you can install:

- **MinGW-w64** (recommended)
- **MSYS2** with GCC
- **Visual Studio Build Tools**

### Installation (Windows with MinGW-w64)

1. Download MinGW-w64 from: https://www.mingw-w64.org/
2. Install to a location like `C:\mingw64`
3. Add to your PATH: `C:\mingw64\bin`
4. Verify installation:
   ```bash
   gcc --version
   g++ --version
   ```

### Running Tests

Once you have a C/C++ compiler installed:

```bash
# Run all tests
pio test -e native

# Run specific test
pio test -e native -f test_personality_manager

# Run with verbose output
pio test -e native -vvv
```

### Expected Output

When tests pass, you should see:

```
=============================================================== SUMMARY ===============================================================
Environment    Test                      Status    Duration
-------------  ------------------------  --------  ------------
native         test_button_input         PASSED    00:00:XX.XXX
native         test_personality_manager  PASSED    00:00:XX.XXX
================== 2 test cases: 2 succeeded in 00:00:XX.XXX ==================
```

## Building for Arduino UNO

To build the firmware for the Arduino UNO:

```bash
pio run -e uno
```

This will compile the code and generate `firmware.hex` in `.pio/build/uno/`.

## Hardware Setup

### Pin Configuration

- **Button Pin**: GPIO 2 (with internal pull-up)
- **Servo Pin**: GPIO 9 (PWM)

### Wiring

- Button: Connect between pin 2 and GND
- Servo:
  - Signal → Pin 9
  - Power → 5V
  - Ground → GND

## Adding New Personalities

To add a new personality:

1. Add a new class in [`lib/Personality/src/PersonalityImpl.h`](lib/Personality/src/PersonalityImpl.h):

   ```cpp
   class MyPersonality : public Personality {
   public:
       void execute(uint8_t servoPin) override;
       const char* getName() const override { return "MyName"; }
       unsigned long getDuration() const override { return 1000; }
   };
   ```

2. Implement in [`lib/Personality/src/PersonalityImpl.cpp`](lib/Personality/src/PersonalityImpl.cpp):

   ```cpp
   void MyPersonality::execute(uint8_t servoPin) {
       // Your servo control logic here
   }
   ```

3. Add to the personality array in [`src/main.cpp`](src/main.cpp):

   ```cpp
   MyPersonality myPersonality;
   Personality* personalities[] = {
       &fastPersonality,
       &slowPersonality,
       &jitteryPersonality,
       &lazyPersonality,
       &myPersonality  // Add here
   };
   ```

4. Update the array size in PersonalityManager initialization:
   ```cpp
   PersonalityManager personalityManager(personalities, 5);  // Changed from 4 to 5
   ```

## Troubleshooting

### Tests won't compile

- Ensure you have a C/C++ compiler installed and in your PATH
- Run `gcc --version` to verify

### Tests fail with "gcc not found"

- Install MinGW-w64 or another C/C++ compiler
- Add the compiler's bin directory to your system PATH
- Restart your terminal/IDE

### Arduino build fails

- Ensure you have the Arduino framework installed: `pio platform install atmelavr`
- Check that all library includes are correct

## Architecture Notes

The system uses an abstract base class pattern for personalities, allowing:

- Easy addition of new personality types
- Unit testing without hardware
- Clear separation of concerns
- Extensibility for future features

The PersonalityManager handles cycling through personalities, ensuring each activation uses a different behavior pattern.
