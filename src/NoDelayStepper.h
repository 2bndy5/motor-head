#ifndef MOTOR_HEAD_NO_DELAY_STEPPER_H_
#define MOTOR_HEAD_NO_DELAY_STEPPER_H_
#include "platformConfig.h"

namespace MotorHead {

/** The type of stepper motor design. */
enum StepperType
{
    /** Only 1 of the motor's coils is energized for each step */
    Wave = 0,
    /** Energizes 2 of the motor's coils for each step */
    Full = 1,
    /** The total energized motor coils alternates between 1 and 2 for each step */
    Half = 2,
};

class NoDelayStepper
{
public:
    NoDelayStepper(
        unsigned char pins[], StepperType stepType = StepperType::Half,
        long stepsPerRev = 4096, double degreesPerStep = 0.087890625, short maxRPM = 60
    );
    /** return _steps counter */
    long getSteps();
    /** calc angle */
    double getAngle();
    /** return position (percentage) */
    double getValue();
    /** set target using percentage [-100,100] */
    void go(double);
    /** set target steps per revolution */
    void goSteps(int);
    /** set target steps via angle */
    void goAngle(double);
    bool isGoing();
    /** Call this on every iteration of the main `loop()`. */
    void sync();

private:
    void resetZeroAngle();
    void releasePins();
    /** step the iterator +-1 */
    short step_it(bool);
    /** write pin outputs according to steps counter */
    void write();
    /** wrap variable it w/in specified max and min */
    template<typename T>
    T wrap_it(T max, T min = 0, T it = NULL);
    /** wrap angle to range [0,360] */
    double wrap_angle(short theta);
    /** set ending time to hold between steps */
    void set_delay();
    /** is ClockWise shorter than counter-ClockWise? */
    bool isCW();
    const unsigned char NumbPins = 4;
    /** delta time */
    unsigned long _dt;
    /** Steps Per Revolution */
    long stepsPerRev;
    /** steps counter */
    long _steps;
    /** steps destination */
    long target;
    /** iterator */
    char _it;
    /** stepper type */
    StepperType sType;
    /** Rotations Per Minute
     *
     * This may get deprecated.
     */
    short RPM;
    /** array of pins to motor (digital outputs) */
    unsigned char* pins;
    double degreesPerStep;
};

} // namespace MotorHead

#endif // MOTOR_HEAD_NO_DELAY_STEPPER_H_
