#include "motors.h"
#include "platformConfig.h"

namespace MotorHead {

Solenoid::Solenoid(gpio_pin_t forwardPin, gpio_pin_t backwardPin)
{
    forwardPin = forwardPin;
    backwardPin = backwardPin;
    GPIOClass::open(forwardPin, Direction::Output);
    if (backwardPin != NULL_PIN) {
        GPIOClass::open(backwardPin, Direction::Output);
    }
}

void Solenoid::go(bool strength)
{
    motorOutput = strength;
    write();
}

void Solenoid::write()
{
    GPIOClass::write(forwardPin, (Level)motorOutput);
    if (backwardPin != NULL) {
        GPIOClass::write(backwardPin, (Level)!motorOutput);
    }
}

UniMotor::UniMotor(gpio_pin_t motorPin)
{
    motorPin = motorPin;
    GPIOClass::open(motorPin, Direction::Output);
}

void UniMotor::go(uint8_t speed)
{
    motorOutput = speed;
    write();
}

void UniMotor::write() { GPIOClass::analogWrite(motorPin, motorOutput); }

BiMotor::BiMotor(gpio_pin_t forwardPin, gpio_pin_t backwardPin)
{
    forwardPin = forwardPin;
    GPIOClass::open(forwardPin, Direction::Output);
    backwardPin = backwardPin;
    GPIOClass::open(backwardPin, Direction::Output);
}

void BiMotor::go(int16_t speed)
{
    motorOutput = speed < -255 ? -255 : (speed > 255 ? 255 : speed);
    write();
}

void BiMotor::write()
{
    if (motorOutput > 0) {
        GPIOClass::analogWrite(forwardPin, static_cast<uint8_t>(motorOutput));
        GPIOClass::analogWrite(backwardPin, 0);
    }
    else if (motorOutput < 0) {
        GPIOClass::analogWrite(forwardPin, 0);
        GPIOClass::analogWrite(backwardPin, static_cast<uint8_t>(motorOutput));
    }
    else {
        GPIOClass::analogWrite(forwardPin, 0);
        GPIOClass::analogWrite(backwardPin, 0);
    }
}

PhasedMotor::PhasedMotor(gpio_pin_t energyPin, gpio_pin_t directionPin)
    : BiMotor(energyPin, directionPin)
{
}

void PhasedMotor::write()
{
    if (motorOutput > 0) {
        GPIOClass::analogWrite(forwardPin, static_cast<uint8_t>(motorOutput));
        GPIOClass::write(backwardPin, Level::High);
    }
    else if (motorOutput < 0) {
        GPIOClass::analogWrite(forwardPin, static_cast<uint8_t>(abs(motorOutput)));
        GPIOClass::write(backwardPin, Level::Low);
    }
    else {
        GPIOClass::analogWrite(forwardPin, 0);
    }
}

template<typename OutType>
void SmoothMotor<OutType>::go(OutType speed)
{
    targetSpeed = speed < -255 ? -255 : (speed > 255 ? 255 : speed);
    initSmooth = millis();
    initSpeed = motorOutput;
    double of_dt = abs((double)(targetSpeed - initSpeed) / 510.0);
    endSmooth = initSmooth + of_dt * rampTime;
    SmoothMotor<OutType>::sync();
}

template<typename OutType>
void SmoothMotor<OutType>::sync()
{
    unsigned long now = millis();
    if (endSmooth != initSmooth && now > endSmooth && rampTime) {
        double ds = (1 - cos(now / (double)(endSmooth - initSmooth) * PI)) / 2;
        write((int16_t)(ds * (targetSpeed - initSpeed) + initSpeed));
    }
    else {
        write(targetSpeed);
    }
}

void UniMotorSmooth::go(uint8_t speed) { SmoothMotor::go(speed); }

} // namespace MotorHead
