#ifndef MOTOR_HEAD_DRIVETRAIN_H_
#define MOTOR_HEAD_DRIVETRAIN_H_

#include "platformConfig.h"
#include "motors.h"

namespace MotorHead {

class Drivetrain
{
public:
    Drivetrain() : maxSpeed(100) {};
    virtual void sync() = 0;
    short maxSpeed;
};

/// @brief A drivetrain in which 2 motors are used to propel and steer in tandem.
class Tank : public Drivetrain
{
public:
    Tank(BiMotor* leftMotor, BiMotor* rightMotor)
        : leftMotor(leftMotor), rightMotor(rightMotor) {};
    void go(short x, short y);
    void sync();

protected:
    BiMotor* leftMotor;
    BiMotor* rightMotor;
};

/// @brief A drivetrain in which 1 motor is used to steer and another motor is used to
/// propel.
class GuidedPropeller : public Drivetrain
{
public:
    GuidedPropeller(BiMotor* propelMotor, BiMotor* steerMotor)
        : propelMotor(propelMotor), steerMotor(steerMotor) {};
    void go(short x, short y);
    void sync();

protected:
    BiMotor* propelMotor;
    BiMotor* steerMotor;
};

/**
 * @brief A drivetrain in which 4 motors drive mecanum wheels that are used to steer and
 * propel in tandem.
 *
 * This is similar to `Tank` drivetrain, but the wheel type allows for more
 * maneuverability.
 */
class Mecanum : public Drivetrain
{
public:
    Mecanum(
        BiMotor* leftFront, BiMotor* rightFront, BiMotor* rightRear, BiMotor* leftRear
    )
        : leftFront(leftFront),
          rightFront(rightFront),
          rightRear(rightRear),
          leftRear(leftRear) {};
    void go(short x, short y, bool strafe);
    void sync();

protected:
    BiMotor* leftFront;
    BiMotor* rightFront;
    BiMotor* rightRear;
    BiMotor* leftRear;
};

} // namespace MotorHead

#endif // MOTOR_HEAD_DRIVETRAIN_H_
