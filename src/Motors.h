#ifndef MOTOR_HEAD_MOTORS_H_
#define MOTOR_HEAD_MOTORS_H_

#include "platformConfig.h"
namespace MotorHead {

/**
 * @brief An abstract class to describe the basic functionality of a motor.
 */
template<typename OutType>
class Motor
{
public:
    Motor() : motorOutput(0) {};
    /**
     * @brief Make the motor move.
     *
     * @param speed The speed to write to the motor's pin(s).
     * This can be negative to go backward or positive to go forward.
     * A zero value stops the motor. This value is clamped to range [-255, 255].
     */
    virtual void go(OutType speed) = 0;

    /**
     * @brief Is the motor moving?
     *
     * @returns The usable value passed to `go()`.
     */
    bool isGoing() { return motorOutput != 0; };

    void sync() {};

    friend SmoothMotor;

protected:
    virtual void write() = 0;
    OutType motorOutput;
};

/**
 * @brief A class to drive a solenoid.
 *
 * @note This object uses absolute force (digital output signals).
 * No PWM signal is used to drive a basic solenoid.
 */
class Solenoid : Motor<bool>
{
public:
    /**
     * @brief Create a solenoid object.
     *
     * @param forwardPin The GPIO number of the pin connected to the solenoid
     * that (when energized) causes the solenoid to go forward. Must be PWM capable.
     * @param backwardPin
     * @parblock
     * The GPIO number of the pin connected to the solenoid
     * that (when energized) causes the solenoid to go backward. Must be PWM capable.
     *
     * If this parameter is not specified (or given a @ref NULL_PIN value), then the
     * solenoid will be simply de-energized when calling `Solenoid::go(false)`. This is
     * not the same as stopping a motor. Depending on the solenoid's design, a
     * de-energized solenoid can be manipulated by other forces (a spring for example).
     * @endparblock
     */
    Solenoid(gpio_pin_t forwardPin, gpio_pin_t backwardPin = NULL_PIN);

    /**
     * @brief Make the solenoid move.
     *
     * @see Review note about the `backwardPin` parameter to the `Solenoid()` constructor.
     */
    void go(bool strength);

private:
    void write();
    gpio_pin_t forwardPin;
    gpio_pin_t backwardPin;
};

/**
 * @brief A class to drive a unidirectional motor.
 */
class UniMotor : public Motor<uint8_t>
{
public:
    /**
     * @brief Create a UniMotor object.
     *
     * @param motorPin The GPIO number of the pin connected to the motor
     * that (when energized) causes the motor to move. Must be PWM capable.
     */
    UniMotor(gpio_pin_t motorPin);

    /**
     * @brief Make the motor move.
     *
     * @param speed Since this class uses a motor to only go in 1 direction,
     * the value passed here is clamped to range [0, 255].
     */
    void go(uint8_t speed);

private:
    void write();
    gpio_pin_t motorPin;
};

/**
 * @brief A class to drive a bidirectional motor
 */
class BiMotor : public Motor<int16_t>
{
public:
    /**
     * @brief Create a bidirectional motor object.
     *
     * @param forwardPin The GPIO number of the pin connected to the motor
     * that (when energized) causes the motor to go forward. Must be PWM capable.
     * @param backwardPin The GPIO number of the pin connected to the motor
     * that (when energized) causes the motor to go backward. Must be PWM capable.
     */
    BiMotor(gpio_pin_t forwardPin, gpio_pin_t backwardPin);

    /**
     * @brief Make the motor move.
     *
     * @param speed The speed to write to the motor's pin(s).
     * This can be negative to go backward or positive to go forward.
     * A zero value stops the motor. This value is clamped to range [-255, 255].
     */
    void go(int16_t speed);

protected:
    void write();
    gpio_pin_t forwardPin;
    gpio_pin_t backwardPin;
};

/**
 * @brief A class to drive a phased bidirectional motor.
 */
class PhasedMotor : public BiMotor
{
public:
    /**
     * @brief Create a phased bidirectional motor object.
     *
     * @param energyPin The GPIO number of the pin connected to the motor
     * that (when energized) causes the motor to move. Must be PWM capable.
     * @param directionPin The GPIO number of the pin connected to the motor
     * that (when energized) causes the motor to go backward. Must be capable of
     * a digital output signal.
     */
    PhasedMotor(gpio_pin_t energyPin, gpio_pin_t directionPin);

protected:
    void write();
};

/**
 * @brief A mixin class to smooth signals sent to a motor.
 *
 * @see This class is mixed in with
 * - @ref UniMotorSmooth
 * - @ref BiMotorSmooth
 * - @ref PhasedMotorSmooth
 */
template<typename OutType>
class SmoothMotor
{
public:
    SmoothMotor()
        : rampTime(2000), targetSpeed(0), initSpeed(0), initSmooth(0), endSmooth(0) {};
    void go(OutType speed);
    /**
     * @brief Call this function on every iteration of the main `loop()`.
     */
    void sync();

    /**
     * @brief The time taken to smoothly transition the motor's signals.
     *
     * By default this is 2000 milliseconds. Set this to a lesser value to decrease the
     * smoothness.
     */
    uint32_t rampTime;

protected:
    OutType targetSpeed, initSpeed;
    unsigned long initSmooth, endSmooth;
};

/**
 * @brief Same as @ref UniMotor but with output signals smoothed over a period of time.
 *
 * @see Adjust the period of smoothing time with @ref rampTime.
 */
class UniMotorSmooth : public UniMotor, public SmoothMotor<uint8_t>
{
public:
    /**
     * @brief Create a UniMotorSmooth object.
     *
     * @param motorPin The GPIO number of the pin connected to the motor
     * that (when energized) causes the motor to move. Must be PWM capable.
     */
    UniMotorSmooth(gpio_pin_t motorPin) : UniMotor(motorPin) {};
    void go(uint8_t speed) { SmoothMotor<uint8_t>::go(speed); };
};

/**
 * @brief Same as @ref BiMotor but with output signals smoothed over a period of time.
 *
 * @see Adjust the period of smoothing time with @ref rampTime.
 */
class BiMotorSmooth : public BiMotor, public SmoothMotor<int16_t>
{
public:
    BiMotorSmooth(gpio_pin_t forwardPin, gpio_pin_t backwardPin)
        : BiMotor(forwardPin, backwardPin) {};
    void go(int16_t speed) { SmoothMotor<int16_t>::go(speed); };
};

/**
 * @brief Same as @ref PhasedMotor but with output signals smoothed over a period of time.
 *
 * @see Adjust the period of smoothing time with @ref rampTime.
 */
class PhasedMotorSmooth : public PhasedMotor, public SmoothMotor<int16_t>
{
public:
    PhasedMotorSmooth(gpio_pin_t energyPin, gpio_pin_t directionPin)
        : PhasedMotor(energyPin, directionPin) {};
    void go(int16_t speed) { SmoothMotor<int16_t>::go(speed); };
};

} // namespace MotorHead

#endif // MOTOR_HEAD_MOTORS_H_
