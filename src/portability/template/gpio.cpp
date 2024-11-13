#ifndef ARDUINO
    #include "gpio.h"

namespace MotorHead {
GPIOClass::GPIOClass() {}

void GPIOClass::open(gpio_pin_t pinNumber, Direction direction, bool pwmCapable) {}

void GPIOClass::close(gpio_pin_t pinNumber) {}

Level GPIOClass::read(gpio_pin_t pinNumber) { return Level::Low; }

uint8_t GPIOClass::analogRead(gpio_pin_t pinNumber) { return 0; }

void GPIOClass::write(gpio_pin_t pinNumber, Level value) {}

void GPIOClass::analogWrite(gpio_pin_t pinNumber, uint8_t level) {}

GPIOClass::~GPIOClass() {}

} // namespace MotorHead

#endif // ARDUINO
