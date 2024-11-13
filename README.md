# Motor-Head

A driver library for various types of motors in various drivetrain configurations.

> [!tip]
> API documentation is hosted at <https://2bndy5.github.io/motor-head>.

## Supported hardware

> [!caution]
> Due to DC motors' power requirements, users will still need
> peripheral breakout boards to drive multiple motors.
>
> At time of writing, any breakout board that takes PWM and/or digital output signals is supported (ie L293D).
> Support is planned (but not implemented) for breakout boards that interface with the MCU using I2C or SPI.

### Motor types supported

This library supports uni-directional and bi-direction DC motors including

- [x] phased motors
- [x] stepper motors
- [x] solenoids

### Drivetrain configurations

This library also provides convenient classes that implement popular drivetrain configurations including

- [x] differential (tank)
- [x] guided propulsion (like a car)
- [x] mecanum wheels (4WD with special treads)

## Platforms supported

This library aims to support the following platforms:

- [x] Arduino
- [x] PlatformIO
- [ ] Linux
- [ ] Pico SDK

### Python bindings

This is planned but not implemented yet.
