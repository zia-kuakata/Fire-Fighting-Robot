# Software Guide

Both sketches use only the Arduino core (no external libraries) and target AVR boards
such as the Uno/Nano.

## 1. `fire_fighting_robot` (autonomous)

File: [`firmware/fire_fighting_robot/fire_fighting_robot.ino`](../firmware/fire_fighting_robot/fire_fighting_robot.ino)

### 1.1 Structure

| Element | Description |
|---------|-------------|
| `flameSensor1Pin/2/3` | `A0`, `A1`, `A2` - analog flame inputs |
| `trigPin`, `echoPin` | `13`, `12` - HC-SR04 |
| `RELAY_PIN` | `7` - pump relay (active-LOW) |
| `IN1..IN4` | `5, 6, 9, 10` - L298N inputs, all PWM-capable |
| `setup()` | Starts Serial (9600), sets pin modes |
| `loop()` | Reads sensors, prints telemetry, selects a behaviour |
| `moveForward(speed)` | `IN1`, `IN3` = PWM; `IN2`, `IN4` = 0 |
| `moveBackward(speed)` | `IN2`, `IN4` = PWM; `IN1`, `IN3` = 0 |

Stopping is done by calling either function with `speed = 0`.

### 1.2 Sensing

- **Flame sensors** - `analogRead()` returns 0-1023. The sketch comment states that
  **lower values mean fire**. This is the usual behaviour for analog flame modules
  (output voltage falls as IR intensity rises), but confirm it on your hardware.
- **Distance** - a 10 microsecond pulse is sent on `TRIG`; `pulseIn(echoPin, HIGH)`
  measures the echo and `distance = duration * 0.034 / 2` converts to cm (speed of
  sound about 0.034 cm/microsecond).

### 1.3 Decision logic

Each iteration evaluates three mutually exclusive branches:

| # | Condition | Action |
|---|-----------|--------|
| 1 | `flame1 > 40 && flame2 > 45 && flame3 > 40` **and** `distance > 25` | `moveBackward(150)` |
| 2 | else if `distance <= 30` | Stop; while `flame1 <= 50 && flame2 <= 55 && flame3 <= 50`: pump ON and re-read flames; then pump OFF |
| 3 | else | Stop motors, pump OFF |

After every iteration: `delay(500)`.

### 1.4 Telemetry format

Printed once per loop at 9600 baud:

```
Flame 1: 812	Flame 2: 790	Flame 3: 805	Distance: 47
```

Values are tab-separated, so the output can be pasted into a spreadsheet or plotted with
the Arduino Serial Plotter (labels excluded).

### 1.5 Interpretation of the current behaviour

Read literally, the code drives the robot while **all three sensors read above their
thresholds** (which, with "lower = fire", means *no strong flame*), and pumps only when
the robot is within 30 cm of something and **all three sensors read at or below** their
pump thresholds. This is different from the report's "detect, turn toward and approach
the fire". It may reflect a sensor polarity that differs from the comment, or an
unfinished prototype. Verify on hardware - see [known-issues.md](known-issues.md), items
K-03 and K-04.

## 2. `manual_joystick_control` (test)

File: [`firmware/manual_joystick_control/manual_joystick_control.ino`](../firmware/manual_joystick_control/manual_joystick_control.ino)

A bring-up sketch that lets you spin each motor with a joystick axis:

| Joystick axis | Motor | Behaviour |
|---------------|-------|-----------|
| X (`A0`) | Motor A (`ENA`=11, `IN1`=9, `IN2`=8) | `< 400` one direction, `> 600` the other, speed scaled by `map()` to 0-255 |
| Y (`A1`) | Motor B (`ENB`=10, `IN3`=7, `IN4`=6) | Same scheme |

- Dead-zone is 400-600 (motor off).
- The two axes drive two motors **independently** (it is not a differential "steer +
  throttle" mix), which is ideal for verifying wiring and rotation direction but not for
  driving the robot.
- Readings exactly equal to `400` or `600` match no branch and keep the previous
  output; this is harmless for testing (see K-11).

## 3. Build and dependencies

| Item | Value |
|------|-------|
| Core | `arduino:avr` |
| Default FQBN | `arduino:avr:uno` |
| Libraries | none |

CI compiles both sketches on every push and pull request
(`.github/workflows/arduino-ci.yml`).

## 4. Coding conventions for contributions

- Keep pin numbers and thresholds as named `const` values at the top of the file.
- Avoid blocking loops; prefer `millis()`-based timing.
- Comments must match the code (several current comments do not - see K-06).
- Format with the Arduino IDE auto-format (Ctrl+T), 2-space indent.
