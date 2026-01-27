# Slave firmware hardware review (pin and resource check)

## Scope
Review of `firmwares/slave/slave.ino` against the previously validated single-function sketches to check pin usage, shared peripherals (SPI/Timer1/interrupts), and possible conflicts.

## Pin map comparison

| Function | Pin | Notes | Source |
| --- | --- | --- | --- |
| RTD inner CS | 2 | Matches `onlyrtd` CS2. | `slave.ino`, `onlyrtd.ino` |
| RTD external CS | 5 | Matches `onlyrtd` CS1. | `slave.ino`, `onlyrtd.ino` |
| Peltier PWM | 4 | Same as `heaterAndPeltier`. | `slave.ino`, `heaterAndPeltier.ino` |
| Peltier switches | 14, 19 | Same as `heaterAndPeltier`. | `slave.ino`, `heaterAndPeltier.ino` |
| Peltier enable | 25 | Same as `heaterAndPeltier`. | `slave.ino`, `heaterAndPeltier.ino` |
| Heater enable | 10 | Same as `heaterAndPeltier`. | `slave.ino`, `heaterAndPeltier.ino` |
| Heater PWM | A4 | Same as `heaterAndPeltier`. | `slave.ino`, `heaterAndPeltier.ino` |
| Motor select | 7 | Same as `onlyMotor`. | `slave.ino`, `onlyMotor.ino` |
| Motor direction | 26 | Same as `onlyMotor`. | `slave.ino`, `onlyMotor.ino` |
| Motor PWM | 23 | Same as `onlyMotor`. | `slave.ino`, `onlyMotor.ino` |
| Motor feedback | 3 | Same as `onlyMotor` (INT pin). | `slave.ino`, `onlyMotor.ino` |
| USB enable | 24 | Same as `heaterAndPeltier`. | `slave.ino`, `heaterAndPeltier.ino` |

## Shared resources & interactions

### SPI (MAX31865)
- Uses CS pins 2 and 5, matching the RTD-only test sketch. This avoids pin overlap with motor/heater/peltier outputs.
- Pin 10 (SS) is used for `HEATER_EN_PIN`, but it is configured as an OUTPUT. On AVR, as long as SS is output, the SPI master role is maintained even if the pin is pulled low. The code keeps it as OUTPUT at all times, so this is safe.

### Timer1 ISR (software PWM)
- Timer1 compare ISR is used to drive **three** software PWM outputs (peltier, heater, motor).
- `T1_PERIOD_TICKS` is set to 532, which is consistent with ~30 kHz at 16 MHz or ~15 kHz at 8 MHz. This matches the overall intent of the earlier sketches (software PWM on Timer1), but now a single ISR toggles three pins.
- **Potential risk:** if the MCU clock or ISR load changes, jitter can show up in software PWM. Functionally, it should still work, but worth keeping an eye on if PWM outputs appear noisy under load.

### Interrupt for motor feedback
- Motor feedback uses `attachInterrupt` on pin 3 (INT1 on Mega-class boards). This is distinct from SPI and PWM pins, so no direct conflicts are expected.

## Potential hardware-risk callouts

1. **Heater enable on pin 10 (SPI SS):**
   - Safe as long as the pin remains configured as OUTPUT, which is done in `setup()`. If future refactors switch it to input, SPI could drop into slave mode; keep the OUTPUT configuration.

2. **Timer1 software PWM for three outputs:**
   - Running three software PWMs in one ISR is feasible, but the interrupt load increases. If the board runs at 8 MHz, the ISR runs at ~15 kHz; at 16 MHz, ~30 kHz. Either is likely okay, but if you observe heating/motor PWM irregularities when all subsystems are active, this shared Timer1 ISR is the first suspect.

3. **Pin 19 usage:**
   - Pin 19 is used for a peltier switch. On ATmega2560, pin 19 is RX1; that is generally safe as long as Serial1 is not used. If Serial1 is ever required later, this would conflict.

## Summary
- The pin map in `slave.ino` matches the working standalone test sketches; no direct pin collisions are apparent.
- The only realistic hardware risk is the **shared Timer1 ISR load** when all subsystems run together, plus the **SS pin 10** requirement to remain OUTPUT.
- No immediate hardware conflicts are evident based on the current pin assignments.
