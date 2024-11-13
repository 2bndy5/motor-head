#include "gpio.h"
#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

namespace MotorHead {
    GPIOClass::GPIOClass() {}

    void GPIOClass::open(gpio_pin_t pinNumber, Direction direction, bool pwmCapable)
    {
        // no standardized way to verify pin capabilities in Arduino framework
        (void)pwmCapable;

        pinMode(pinNumber, direction);
    }

    void GPIOClass::close(gpio_pin_t pinNumber)
    {
        // pins can be appropriated in Arduino framework.
    }

    Level GPIOClass::read(gpio_pin_t pinNumber) { return digitalRead(pinNumber); }

    uint8_t GPIOClass::analogRead(gpio_pin_t pinNumber) { return analogRead(pinNumber); }

    void GPIOClass::write(gpio_pin_t pinNumber, Level value)
    {
        digitalWrite(pinNumber, value);
    }

    void GPIOClass::analogWrite(gpio_pin_t pinNumber, uint8_t level)
    {
        analogWrite(pinNumber, level);
    }

    GPIOClass::~GPIOClass()
    {
        // no way to de-initialize pins in Arduino framework
    }

} // namespace MotorHead

#ifdef __cplusplus
}
#endif
