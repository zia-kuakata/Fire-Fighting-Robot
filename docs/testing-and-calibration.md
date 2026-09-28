# Testing and Calibration

Work through these stages in order. **Wheels off the ground and pump disconnected** until
stage 4.

## Stage 1 - Motor driver (joystick sketch)

1. Upload `manual_joystick_control`.
2. Move the joystick X axis: motor A should turn one way for `< 400`, the other for `> 600`.
3. Repeat with Y for motor B.
4. Note which direction is "forward" for each motor. If a motor turns the wrong way,
   swap its two motor wires on the L298N (or swap the `IN` pins in code).

Pass: both motors respond smoothly in both directions; speed increases with deflection.

## Stage 2 - Sensor read-out

1. Upload `fire_fighting_robot`, keep the motors and pump disconnected.
2. Open the Serial Monitor at **9600 baud**.
3. Record a **baseline** for each flame sensor in normal room light and with no flame.
4. Hold a small lit candle ~30 cm away, record the values, then move to ~15 cm.
5. Note whether the reading **falls or rises** near a flame (the code assumes it falls).
6. Point an object at the HC-SR04 at 10, 20, 30, 50 cm and compare with a ruler.

Record results:

| Sensor | No flame | Flame ~30 cm | Flame ~15 cm |
|--------|----------|--------------|--------------|
| Flame 1 (A0) | | | |
| Flame 2 (A1) | | | |
| Flame 3 (A2) | | | |

| Ruler distance | Reported `Distance` |
|----------------|---------------------|
| 10 cm | |
| 20 cm | |
| 30 cm | |
| 50 cm | |

Tip: turn each module's onboard potentiometer so the analog values are similar across the
three sensors; this simplifies choosing common thresholds.

## Stage 3 - Threshold selection

Choose values from your recorded table:

- **Drive threshold** - between the no-flame baseline and the weakest flame you want to
  react to.
- **Pump threshold** - a value reached only when the flame is within spray range.
- Keep a gap (hysteresis) between drive and pump thresholds to avoid chattering.

Edit the constants in `loop()` and re-upload. Repeat stage 2 to confirm.

## Stage 4 - Pump test (no flame)

1. Connect pump and relay to their own supply; tank filled, nozzle aimed into a bowl.
2. Verify `HIGH` on `D7` = pump **off** and `LOW` = pump **on** (active-LOW relay).
3. Power on and watch for a **brief pump pulse at start-up** (see K-08).
4. Check for leaks and keep water away from electronics.

## Stage 5 - Integrated test (supervised)

Perform on a non-flammable surface with a fire extinguisher at hand, one small candle,
and someone ready to cut power.

| # | Scenario | Expected |
|---|----------|----------|
| 1 | No flame, open floor | Behaviour matches the branch table in [software.md](software.md) |
| 2 | Obstacle at < 25 cm, no flame | Robot stops, pump off |
| 3 | Flame in front, distance <= 30 cm | Robot stops, pump runs until sensors clear |
| 4 | Flame removed during spraying | Pump turns off within one sensor read |
| 5 | Ultrasonic sensor disconnected | Note behaviour (see K-09) |
| 6 | Flame to left / right | Note behaviour - direction finding is **not** implemented |

## Test log template

```
Date:
Firmware commit / version:
Board:                Supply voltages (logic / motor / pump):
Thresholds used:
Scenario #:           Result (pass / fail):        Notes:
```
