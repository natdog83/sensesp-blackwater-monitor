# SensESP sailboat black-water tank monitor

Firmware and a complete build guide for a five-point, non-contact black-water tank monitor using:

- five XKC-Y25-V capacitive liquid sensors;
- a 38-pin ESP32-WROOM-32D / ESP32 DevKit-style board with CP2102 USB; and
- SensESP 3.1.0 to publish tank level to Signal K.

The design is based on the approach demonstrated by [Techstyleuk's SensESP black-water sensor array](https://github.com/Techstyleuk/SensESP-blackwater-tank-sensor-array), with updated Signal K paths, safer ESP32 pins, a shared regulated 5 V supply, explicit 5 V-to-3.3 V signal interfaces, sensor-state diagnostics, invalid-pattern detection, host tests, a serial hardware-test image, and automated builds.

## Start here

Read the **[full wiring, bench-test, installation, and deployment tutorial](docs/INSTALLATION.md)** before connecting power.

## Quick commands

```bash
# Check the level algorithm on your computer
pio test -e native

# Flash the serial-only hardware test image
pio run -e esp32dev-test -t upload
pio device monitor -b 115200

# Build and flash the production SensESP image
pio run -e esp32dev -t upload
pio device monitor -b 115200
```

## Safety-critical electrical point

The XKC-Y25-V requires a 5–24 V supply and is not specified to operate from the ESP32 `3V3` pin. Power the sensors and ESP32 board in parallel from the documented fused, regulated 5 V rail. The yellow sensor output can rise toward 5 V, so **never connect it directly to an ESP32 GPIO.** Use the documented transistor interface or a correctly engineered equivalent.

This is a DIY monitoring aid, not a certified overfill-prevention or safety system. Verify operation manually and comply with applicable marine electrical, sanitation, and vessel-manufacturer requirements.

## Project layout

```text
include/tank_config.h          GPIOs, paths, timing, and level thresholds
include/tank_logic.h           Pure tank-level and pattern-validity logic
src/main.cpp                   Production SensESP firmware
src/sensor_test.cpp            Serial-only hardware test firmware
test/test_tank_logic/          Host-side calculation tests
docs/INSTALLATION.md           Detailed tutorial
docs/wiring.svg                Wiring overview
.github/workflows/build.yml    Continuous build and tests
```

## References

- [SensESP getting started guide](https://signalk.org/SensESP/pages/getting_started/)
- [Signal K black-water tank schema](https://signalk.org/specification/1.7.0/doc/vesselsBranch.html)
- [Espressif ESP32-DevKitC documentation](https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32/esp32-devkitc/user_guide.html)
- [XKC-Y25-V manufacturer datasheet mirror](https://www.electrokit.com/upload/quick/dc/01/adff_XKC-Y25-V-datasheet.pdf)

## License

MIT — see [LICENSE](LICENSE).
