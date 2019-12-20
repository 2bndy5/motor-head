#ifndef Drivetrain_h
#define Drivetrain_h
#include "Motors.h"
#include <arduino.h>

class SmoothDrivetrain{
public:
    SmoothDrivetrain(MotorPool* motors, uint8_t maxSpeed = 100, bool smooth = true);
    void go(short cmds[], bool smooth = true);
    void sync();
    void stop();
    bool isCellerating();

protected:
    short clampPWM(short input);
    MotorPool* motorPool;
    bool smooth;
    uint8_t maxSpeed;
    // short* prev_cmds;
};

class Tank: public SmoothDrivetrain{
    // class for controlling the DC drive motors in tandem
public:
    Tank(MotorPool* motors, uint8_t maxSpeed = 100, bool smooth = true);// c'tor
    void go(short x, short y, bool smooth = true);
private:
    // current speeds for left and right motors
    short right, left;
};

class Automotive: public SmoothDrivetrain{
    // class for controlling the DC drive motors in tandem
public:
    Automotive(MotorPool* motors, uint8_t maxSpeed = 100, bool smooth = true);// c'tor
    void go(short x, short y, bool smooth = true);
private:
    // current speeds for left and right motors
    short forwardBackward, leftRight;
};


class Mecanum: public SmoothDrivetrain{
    // class for controlling the DC drive motors in tandem
public:
    Mecanum(MotorPool* motors, uint8_t maxSpeed = 100, bool smooth = true);// c'tor
    void go(short x, short y, bool strafe, bool smooth = true);
private:
    // current speeds for left and right motors
    short right, left;
};

class Locomotive{
public:
    Locomotive(Solenoid* solenoids, uint8_t switchPin);
    void stop();
    void go(bool reverse = false);
    void sync();
    bool isCellerating();
private:
    bool isForward;
    bool isInMotion;
    Solenoid* solenoids;
    uint8_t switchPin;
};
#endif
