# Firmware

Two independent Arduino sketches. Open **one folder at a time** in the Arduino IDE.

| Folder | Purpose | Original file name |
|--------|---------|--------------------|
| [`fire_fighting_robot/`](fire_fighting_robot) | Autonomous flame / distance / pump logic | `fire_service.ino` |
| [`manual_joystick_control/`](manual_joystick_control) | Joystick motor-driver test | `final_6-3-25.ino` |

The sources are byte-for-byte identical to the originals; only the file names changed
(Arduino requires the sketch folder and `.ino` name to match).

Original SHA-256 (for traceability):

```
fe237e2dc9f547dc75f7f5157f46f047c8c76545cfa5d7553be26e5852d8a632  fire_service.ino
30a66c74e060642222c132dcc1c998a18e22f723992c9866eac8a05f718ace6e  final_6-3-25.ino
```

## Build and upload

```bash
arduino-cli core install arduino:avr
arduino-cli compile --fqbn arduino:avr:uno fire_fighting_robot
arduino-cli upload  --fqbn arduino:avr:uno -p <PORT> fire_fighting_robot
```

Or use the Arduino IDE: open the `.ino`, choose the board and port, press Upload.
Serial Monitor: **9600 baud**.

Details: [`../docs/software.md`](../docs/software.md) | Pins: [`../docs/hardware.md`](../docs/hardware.md)
