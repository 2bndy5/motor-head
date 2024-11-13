#include "drivetrain.h"
#include "platformConfig.h"

namespace MotorHead {

inline short clampPWM(short input)
{
    // return a proper range of [-255, 255]
    return (input < -255 ? -255 : (input > 255 ? 255 : input));
}

void Tank::go(short x, short y)
{
    // make sure x and y are in proper range
    x = clampPWM(x);
    y = clampPWM(y);
    // apply speed governor
    if (abs(y) > maxSpeed * 2.55)
        y = maxSpeed * (y <= 0 ? 2.55 : -2.55);
    short left = y, right = y;
    if (y == 0) { // if not going forward or backward (turn on a dime)
        // re-apply speed governor
        if (abs(x) > maxSpeed * 2.55)
            x = maxSpeed * (x <= 0 ? 2.55 : -2.55);
        right = -1 * x;
        left = x;
    }
    else { // if turning left or right adjust motors' speed accordingly
        if (x > 0)
            right *= (255 - x) / 255.0;
        else if (x < 0)
            left *= ((-255 - x) * -1) / 255.0;
    }
    short cmd[] = {left, right};
    leftMotor->go(left);
    rightMotor->go(right);
}

void Tank::sync()
{
    leftMotor->sync();
    rightMotor->sync();
}

void GuidedPropeller::go(short x, short y)
{
    // make sure x and y are in proper range
    x = clampPWM(x);
    y = clampPWM(y);
    // apply speed governor
    if (abs(y) > maxSpeed * 2.55)
        y = maxSpeed * (y <= 0 ? 2.55 : -2.55);
    propelMotor->go(y);
    steerMotor->go(x);
}

void GuidedPropeller::sync()
{
    propelMotor->sync();
    steerMotor->sync();
}

void Mecanum::go(short x, short y, bool strafe)
{
    x = clampPWM(x);
    y = clampPWM(y);
    // apply speed governor
    if (abs(y) > maxSpeed * 2.55)
        y = maxSpeed * (y <= 0 ? 2.55 : -2.55);
    // assuming left/right axis is null (just going forward or backward)
    short left = y;
    short right = y;
    if (!y) {
        // if forward/backward axis is null
        // re-apply speed governor to only axis with a non-zero value
        if (abs(x) > maxSpeed * 2.55)
            x = maxSpeed * (x >= 0 ? 2.55 : -2.55);
        right = x;
        left = x * -1;
        if (!strafe) { // "turning on a dime" functionality
            short cmds[] = {left, left, right, right};
            leftFront->go(left);
            leftRear->go(left);
            rightRear->go(right);
            rightFront->go(right);
        }
        else { // straight strafing functionality
            leftFront->go(left * -1);
            rightFront->go(right);
            leftRear->go(left);
            rightRear->go(right * -1);
        }
    }
    else {
        //  if forward/backward axis is not null and left/right axis is not null
        //  apply differential to motors accordingly
        if (!strafe) {
            // veer and propel
            float offset = (255 - abs(x)) / 255.0;
            if (y) {
                if (x > 0)
                    right *= offset;
                else if (x < 0)
                    left *= offset;
            }
            leftFront->go(left);
            leftRear->go(left);
            rightRear->go(right);
            rightFront->go(right);
        }
        else {
            // strafe and propel
            if (255 > x && x > 128)
                right *= (255 - abs(x)) / -128;
            else if (0 < x && x <= 128)
                right *= (128 - abs(x)) / 128;
            else if (0 > x && x >= -128)
                left *= (128 - abs(x)) / 128;
            else if (-128 > x && x > -255)
                left *= (255 - abs(x)) / -128;
            if (y > 0) { // if it is strafing right
                leftFront->go(left);
                leftRear->go(right);
                rightRear->go(left);
                rightFront->go(right);
            }
            else { // if it is strafing left
                leftFront->go(right);
                leftRear->go(left);
                rightRear->go(right);
                rightFront->go(left);
            }
        }
    }
}

void Mecanum::sync()
{
    leftFront->sync();
    rightFront->sync();
    rightRear->sync();
    leftRear->sync();
}

} // namespace MotorHead
