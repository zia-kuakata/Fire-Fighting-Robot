# Hardware Guide

## 1. Bill of materials

| # | Component | Qty | Notes |
|---|-----------|-----|-------|
| 1 | Arduino Uno / Nano (ATmega328P) | 1 | Board not stated in the source; pins match Uno/Nano |
| 2 | Analog flame / IR fire sensor module | 3 | Right, front, left (per report) |
| 3 | HC-SR04 ultrasonic sensor | 1 | Used by the autonomous sketch |
| 4 | L298N dual H-bridge motor driver | 1 | Report says "L298"; sketches use L298N-style `IN1-IN4` |
| 5 | DC gear motor | 2 | Left and right drive |
| 6 | Robot chassis, wheels, caster | 1 set | |
| 7 | 1-channel relay module | 1 | Assumed **active-LOW** by the code |
| 8 | Water pump, tubing, nozzle, reservoir | 1 set | Pump must be rated for your supply |
| 9 | Servo motor (e.g. SG90) | 1 | In the report only; **not used by current code** |
| 10 | Analog joystick module | 1 | Only for `manual_joystick_control` |
| 11 | Battery pack(s), switch, jumper wires | - | See power notes |

Quantities other than those stated in the report (3 sensors, 2 motors, 1 servo) are
reasonable assumptions from the firmware, not confirmed values.

## 2. Wiring - autonomous robot (`fire_fighting_robot`)

| From | To (Arduino) | Notes |
|------|--------------|-------|
| Flame sensor 1 `AO` | `A0` | Analog output |
| Flame sensor 2 `AO` | `A1` | |
| Flame sensor 3 `AO` | `A2` | |
| HC-SR04 `TRIG` | `D13` | |
| HC-SR04 `ECHO` | `D12` | HC-SR04 is 5 V - fine on Uno/Nano |
| Relay `IN` | `D7` | Active-LOW: LOW = pump ON |
| L298N `IN1` | `D5` (PWM) | Motor 1 |
| L298N `IN2` | `D6` (PWM) | Motor 1 |
| L298N `IN3` | `D9` (PWM) | Motor 2 |
| L298N `IN4` | `D10` (PWM) | Motor 2 |
| Sensors / HC-SR04 / relay `VCC`, `GND` | `5V`, `GND` | Common ground |

The L298N `ENA` / `ENB` pins must be **enabled** for the robot to move. Since this sketch
uses PWM on `IN1-IN4`, the usual approach is to leave the `ENA`/`ENB` jumpers in place
(always enabled).

> Which flame sensor is "left", "front" and "right" is not defined in the code - the
> report lists right, front, left. Record your actual mounting when you build it.

## 3. Wiring - joystick test (`manual_joystick_control`)

| From | To (Arduino) | Notes |
|------|--------------|-------|
| Joystick `VRx` | `A0` | Controls motor A |
| Joystick `VRy` | `A1` | Controls motor B |
| L298N `ENA` | `D11` (PWM) | Speed of motor A |
| L298N `IN1` / `IN2` | `D9` / `D8` | Direction of motor A |
| L298N `IN3` / `IN4` | `D7` / `D6` | Direction of motor B |
| L298N `ENB` | `D10` (PWM) | Speed of motor B |

Remove the `ENA`/`ENB` jumpers for this sketch: it drives them with PWM.

## 4. Pump switching

```
Arduino D7 ──► Relay IN
Relay VCC/GND ◄── 5 V / GND
Battery (+) ──► Relay COM;  Relay NO ──► Pump (+);  Pump (-) ──► Battery (-)
```

- Use the relay's **NO** (normally open) contact so the pump is off when de-energised.
- Keep the pump supply separate from the Arduino supply, with grounds joined.
- If the pump is a DC motor type, add a flyback diode across it.

## 5. Power

| Rail | Suggested source | Feeds |
|------|------------------|-------|
| Logic | USB or a regulated 5 V supply / battery via Arduino `VIN` | Arduino, sensors, relay coil |
| Motors | Battery pack sized to the motors (check L298N drop, ~2 V) | L298N `+12V`/`VS` |
| Pump | Battery/supply matched to pump rating | Relay contact -> pump |

Always join all grounds. Motors and pump can cause brown-outs on a shared supply; if the
Arduino resets when the motors start, separate the rails.

## 6. Mechanical notes

- Mount the three flame sensors at different angles (left / front / right) at a similar
  height to the flame source; check your module datasheet - many have a detection cone of roughly 60 degrees.
- Mount the HC-SR04 at the front, unobstructed.
- Route the nozzle away from electronics; the servo (when added) should sweep across
  the front sensor's field of view.
