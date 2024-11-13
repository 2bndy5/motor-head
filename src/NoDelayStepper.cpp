#include "NoDelayStepper.h"

namespace MotorHead {

NoDelayStepper::NoDelayStepper(
    unsigned char pins[], StepperType stepType, long stepsPerRev, double degreesPerStep,
    short maxRPM
)
{
    _it = 0; // iterator
    _dt = 0; // time between _steps
    stepsPerRev = stepsPerRev;
    degreesPerStep = degreesPerStep;
    RPM = maxRPM;
    target = 0;
    resetZeroAngle();
    sType = stepType;
    pins = pins; // save array* to object
    // expected size must = 4 as NumbPins is hardcoded to 4
    for (unsigned char i = 0; i < NumbPins; ++i) {
        GPIOClass::open(pins[i], Direction::Output);
        GPIOClass::write(pins[i], Level::Low);
    }
}
void NoDelayStepper::resetZeroAngle() { _steps = 0; }

bool NoDelayStepper::isCW()
{
    long currPos = wrap_it<long>(stepsPerRev, 0, _steps % stepsPerRev);
    if (currPos > target)
        return currPos - target < stepsPerRev / 2;
    else
        return target - currPos > stepsPerRev / 2;
}

short NoDelayStepper::step_it(bool isCw)
{
    if (isCw) {
        _it -= 1;
        _steps -= 1;
    }
    else {
        _it += 1;
        _steps += 1;
    }
}

void NoDelayStepper::sync()
{
    if (target != _steps) {
        unsigned long delta = millis();
        if (delta >= _dt) {
            // Serial.print("delta time = ");Serial.println(delta);
            step_it(isCW());
            write();
            // calc Delta t w/ _smooth()
            /*
            if (target != _steps){
                set_delay(); // set end of wait time between _steps
                // Serial.print("stepper _steps = ");
                // Serial.println(_steps);
            } else {
                Serial.print("stepper finished @ ");
                Serial.print(getAngle());
                Serial.println(" degrees");
            }
            */
        }
    }
}

template<typename T>
T NoDelayStepper::wrap_it(T max, T min, T it)
{
    if (it == NULL)
        it = _it;
    while (it >= max)
        it -= max;
    while (it < min)
        it += max;
    return it;
}

bool NoDelayStepper::isGoing() { return target != NULL ? target != _steps : false; }

void NoDelayStepper::write()
{
    short max_it = 0;
    short next = 0;
    short base = 0;
    switch (sType) {
        case StepperType::Wave:
            max_it = 4;
            _it = wrap_it<short>(max_it);
            for (unsigned char i = 0; i < NumbPins; i++) {
                if (i == _it)
                    GPIOClass::write(pins[i], Level::High);
                else
                    GPIOClass::write(pins[i], Level::Low);
            }
            break;
        case StepperType::Full:
            max_it = 4;
            _it = wrap_it<short>(max_it);
            next = _it + 1 >= NumbPins ? 0 : _it + 1;
            for (unsigned char i = 0; i < NumbPins; i++) {
                if (i == _it || i == next)
                    GPIOClass::write(pins[i], Level::High);
                else
                    GPIOClass::write(pins[i], Level::Low);
            }
            break;
        case StepperType::Half:
            max_it = 8;
            _it = wrap_it<short>(max_it);
            base = _it / 2;
            next = base + 1 <= NumbPins ? base + 1 : 0;
            for (unsigned char i = 0; i < NumbPins; i++) {
                if (i == base || (i == next && _it % 2 == 1))
                    GPIOClass::write(pins[i], Level::High);
                else
                    GPIOClass::write(pins[i], Level::Low);
            }
            break;
        default:
            // should throw an error, but I'm lazy
            break;
    }
}

void NoDelayStepper::set_delay()
{
    _dt = millis() + (unsigned long)(RPM * stepsPerRev / 60000.0);
    /* Serial.print("delay = ");
    Serial.print((RPM * SPR / 60000.0));
    Serial.println(" milliseconds."); */
}

long NoDelayStepper::getSteps() { return _steps; }

double NoDelayStepper::getAngle() { return _steps * degreesPerStep; }
double NoDelayStepper::getValue()
{
    return wrap_it<double>(stepsPerRev / 2, stepsPerRev / -2, _steps) / (stepsPerRev / 2)
           * 100;
}

void NoDelayStepper::releasePins()
{
    for (unsigned char i = 0; i < NumbPins; i++)
        GPIOClass::write(pins[i], Level::Low);
}

// use to map angular values of [-180.0, 180.0] to percentage values of [-100.0, 100.0]
void NoDelayStepper::go(double dest)
{
    if (dest == NULL) {
        resetZeroAngle();
        releasePins();
    }
    else {
        dest = dest < -100.0 ? -100.0 : (dest > 100.0 ? 100.0 : dest);
        target = wrap_it<long>(stepsPerRev, 0, stepsPerRev / 200.0 * dest);
        sync();
    }
}

// meant for telling stepper to turn CW | CCW
// input 'dest' is int of steps to take (or until 0 is passed for 'dest')
void NoDelayStepper::goSteps(int dest)
{
    if (dest == NULL) {
        resetZeroAngle();
        releasePins();
    }
    else {
        target = dest + _steps;
        // Serial.print("motor target: ");Serial.println(target);
        sync();
    }
}

// expects angle between -180 & 180, but will wrap input to [0,360]
void NoDelayStepper::goAngle(double dest)
{
    if (dest == NULL) {
        resetZeroAngle();
        releasePins();
    }
    else {
        target = round(wrap_it<double>(360.0, 0.0, dest) / 360.0 * stepsPerRev);
        sync();
    }
}

} // namespace MotorHead
