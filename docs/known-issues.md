# Known Issues and Improvement Notes

This file records where the **project report** and the **firmware** disagree, plus bugs
and risks found while documenting the code. The original sketches were imported
**unchanged**; nothing below has been fixed in the source yet. Suggested fixes are
illustrative and untested on hardware.

Severity: **High** (affects safety or core function), **Medium**, **Low**.

## A. Report vs. code

| ID | Severity | Issue |
|----|----------|-------|
| K-01 | High | **No servo code.** The report (objectives, sections 4 and 6) describes a servo that sweeps the nozzle. Neither sketch includes `Servo.h` or any servo pin. |
| K-02 | High | **No direction finding or turning.** The report describes determining fire direction from right/front/left sensors and having turn functions. The code reads three sensors but only combines them with logical AND; it has `moveForward`/`moveBackward` only (stop = speed 0). |
| K-10 | Low | **Ultrasonic sensor undocumented in the report.** The firmware uses an HC-SR04 for obstacle detection and stand-off distance; the report's component list omits it. The report also says "L298" while the wiring style matches an L298N module. |

## B. Logic and behaviour (`fire_fighting_robot.ino`)

| ID | Severity | Issue |
|----|----------|-------|
| K-03 | High | **Sensor polarity looks inconsistent.** Comment: "lower values indicate fire". The pump loop is consistent with that (`flame <= 50/55/50`), but the drive branch is titled "if fire is detected" and triggers when all readings are **greater than** `40/45/40`, i.e. when there is *no* strong flame under that convention. Confirm polarity on hardware and align the logic. |
| K-04 | Medium | **"Forward" calls `moveBackward`.** Condition 1 is commented "move forward" but calls `moveBackward(150)`. This may be compensating for motor wiring; if so, swap the motor wires or pins and use `moveForward` so the code reads correctly. |
| K-05 | Medium | **Pump requires all three sensors at once.** The `while` condition uses `&&`, so a flame seen by one or two sensors will not start the pump. For fire-in-front cases you probably want `\|\|`, or `min(flame1, flame2, flame3) <= threshold`. |
| K-06 | Low | **Comments and thresholds disagree.** A comment says "obstacle close (<= 15 cm)" but the code tests `<= 30`; the drive branch tests `> 25`; drive thresholds (40/45/40) and pump thresholds (50/55/50) differ without documented reasoning. Use named constants with comments. |
| K-07 | High | **Blocking pump loop with no time-out.** The `while` loop runs until the sensors clear, with no maximum duration, no obstacle re-check and no manual stop. A stuck sensor or a persistent fire keeps the pump running and the CPU blocked. Add a time limit and a cool-down. |
| K-08 | Medium | **Possible pump pulse at boot.** `RELAY_PIN` is set to `OUTPUT` without first writing `HIGH`. On AVR the pin drives LOW immediately, which turns an active-LOW relay **on** until the first `loop()` pass. Write the OFF level first. |
| K-09 | Medium | **Ultrasonic time-out treated as obstacle.** `pulseIn()` returns `0` if no echo arrives (default time-out 1 s), which makes `distance = 0`. That satisfies `distance <= 30`, so a disconnected or blocked sensor looks like a very close obstacle and stalls each loop for up to a second. Use `pulseIn(echoPin, HIGH, 30000)` and treat `0` as "no reading". |
| K-12 | Low | **Slow response.** `delay(500)` plus `pulseIn` limits reaction time to about half a second per cycle. Consider `millis()` scheduling. |

### Suggested fixes (not applied)

```cpp
// K-08: safe relay start-up
digitalWrite(RELAY_PIN, HIGH);      // OFF for an active-LOW relay
pinMode(RELAY_PIN, OUTPUT);

// K-09: bounded echo wait, 0 = no reading
long duration = pulseIn(echoPin, HIGH, 30000UL);   // ~5 m max
int  distance = duration ? duration * 0.034 / 2 : 999;

// K-07: pump time limit (sketch)
unsigned long start = millis();
while (fireDetected() && millis() - start < 8000UL) {
  digitalWrite(RELAY_PIN, LOW);
}
digitalWrite(RELAY_PIN, HIGH);
```

## C. Test sketch (`manual_joystick_control.ino`)

| ID | Severity | Issue |
|----|----------|-------|
| K-11 | Low | Each axis drives one motor independently (not a steering mix), and joystick values of exactly `400` or `600` match no branch so the previous output persists. Fine for wiring tests; not suitable for driving the robot. Use `<= 400` / `>= 600` and stop the motors explicitly if reused. |

## D. Missing pieces

- No wiring diagram or photos of the built robot.
- No calibration data for the actual sensor modules.
- No automated tests (hardware-dependent); CI only verifies that the sketches compile.
- The source material does not state the Arduino board, supply voltages or motor/pump
  ratings - fill these in `docs/hardware.md` once known.

## E. Suggested implementation order

1. K-03 / K-04: confirm sensor polarity and motor direction on the bench.
2. K-08, K-09, K-07: safety and robustness fixes.
3. K-02: add `turnLeft()`, `turnRight()`, `stopMotors()` and pick direction from the
   lowest sensor reading.
4. K-01: add a servo sweep while the pump runs.
5. K-05, K-06, K-12: tidy thresholds and timing.
