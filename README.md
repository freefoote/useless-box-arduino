# Useless Box

A Platform.io Arduino project implementing a "useless box" with multiple personalities. When the button is activated, a servo turns off the switch using different behavioral patterns that cycle through each activation.

## Features

- **Multiple Personalities**: Four distinct behavioral patterns (Fast, Slow, Jittery, Lazy)
- **Personality Cycling**: Each button press uses a different personality
- **Unit Testable**: Personality system can be tested locally without hardware
- **Modular Architecture**: Clean separation of concerns with reusable libraries
- **Arduino UNO Compatible**: Runs on Arduino UNO with SG90 servo

## Building the actual box

The production "useless box" that this matches was actually made by someone other than me; so I don't have design files for an actual box. Their box was a handmade wooden box which fits the theme of the other items they made. I then contributed the hardware and software for the device.

However, a quick search for "3d print useless box with sg90 servo" should find you plenty of starting points for a useless box. This code should adapt easily to an ESP32 or Arduino Nano if needed.

## AI Disclaimer

Yes, this whole project was largely coded by an LLM based on my instructions. It's got tests at least, and I did have to fiddle with the code a little bit to get it to actually work properly and smoothly. Your mileage may vary, as the saying goes.

## Quick Start

As noted, this uses Platform.io; you can either use `uv` and then `uv tool install platformio`; or use the VS Code extension; or any other method of running Platform.io as you normally would. For work reasons, I was on Windows and thus used Windows, but this should work much more smoothly on a unix like operating system - especially the unit tests.

### Building for Arduino UNO

```bash
pio run -e uno
pio run -t upload -e uno
pio device monitor -e uno
```

### Running Unit Tests

```bash
pio test -e native
```

See [TESTING_GUIDE.md](TESTING_GUIDE.md) for detailed testing instructions.

#### Running Unit Tests on Windows

Because I'm a masochist apparently.

- Install [MSYS2](https://www.msys2.org/)
- Run the MSYS2 UCRT64 terminal.
- Run the command to install gcc: `pacman -S mingw-w64-ucrt-x86_64-gcc`
- Add to the system path.
  - Use the `Edit the system environment variables` option in the Windows search bar.
  - Edit the Path.
  - Add `C:\msys64\ucrt64\bin` to the path.
  - Restart VS Code to get it to see this. (Reload Window is not enough)

## Hardware Requirements

- Arduino UNO
- SG90 Servo Motor
- Push Button
- Jumper wires
- USB cable for programming

## Pin Configuration

- **Button**: GPIO 2 (with internal pull-up)
- **Servo**: GPIO 8 (PWM)

## Project Structure

```
├── lib/
│   ├── Personality/          # Personality system base and implementations
│   ├── PersonalityManager/    # Manages personality cycling
│   └── ButtonInput/           # Button input with debouncing
├── src/
│   └── main.cpp              # Main application
├── test/
│   ├── test_personality_manager/
│   └── test_button_input/
├── platformio.ini            # Platform.io configuration
├── TESTING_GUIDE.md          # Detailed testing guide
└── README.md                 # This file
```

## Personalities

### Fast

- Duration: 500ms
- Behavior: Quick flick of the switch

### Slow

- Duration: 2000ms
- Behavior: Deliberate, methodical movement

### Jittery

- Duration: 1500ms
- Behavior: Nervous, twitchy movements

### Lazy

- Duration: 3000ms
- Behavior: Reluctant, minimal effort

## Architecture

The system uses an abstract base class pattern for personalities:

- **Personality** (abstract): Defines the interface for all personality behaviors
- **PersonalityImpl**: Concrete implementations of each personality
- **PersonalityManager**: Manages cycling through personalities
- **ButtonInput**: Handles button input with debouncing

This architecture allows:

- Easy addition of new personality types
- Unit testing without hardware
- Clear separation of concerns
- Extensibility for future features

## Adding New Personalities

See [TESTING_GUIDE.md](TESTING_GUIDE.md#adding-new-personalities) for instructions on adding new personality types.

## Development Workflow

1. **Design**: Plan your personality behavior
2. **Implement**: Add the personality class to [`lib/Personality/src/PersonalityImpl.h`](lib/Personality/src/PersonalityImpl.h)
3. **Test**: Write unit tests in [`test/`](test/)
4. **Integrate**: Add to the personality array in [`src/main.cpp`](src/main.cpp)
5. **Deploy**: Build and upload to Arduino UNO

## Troubleshooting

See [TESTING_GUIDE.md](TESTING_GUIDE.md#troubleshooting) for common issues and solutions.
