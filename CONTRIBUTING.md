# Contributing

Thanks for helping improve this project!

## Ways to contribute

- Report bugs or hardware findings (sensor polarity, thresholds, wiring photos).
- Fix items from [`docs/known-issues.md`](docs/known-issues.md).
- Improve documentation or add diagrams.

## Workflow

1. Describe the proposed change before starting larger work.
2. Fork the repository and create a branch: `feature/<short-name>` or `fix/<short-name>`.
3. Make your changes. Keep commits focused, e.g. `fix: write relay OFF before pinMode`.
4. Make sure both sketches still compile:
   ```bash
   arduino-cli compile --fqbn arduino:avr:uno firmware/fire_fighting_robot
   arduino-cli compile --fqbn arduino:avr:uno firmware/manual_joystick_control
   ```
5. Update the docs and `CHANGELOG.md` when behaviour, pins or thresholds change.
6. Open a pull request with a clear summary and test notes.

## Code style

- 2-space indentation; use Arduino IDE auto-format.
- Pins and thresholds as named `const` values; comments must match the code.
- No blocking `while` loops on hardware conditions without a time-out.
- Only the Arduino core unless a library is clearly justified (document it).

## Hardware changes

If you change pins, add a component or alter wiring, update the pin tables in
`README.md` and `docs/hardware.md` in the same pull request, and say which board and
modules you tested with.

## Safety

Never submit changes that remove or weaken safety measures without discussion. Test with
small flames, supervision and an extinguisher nearby.
