#include "Drivetrain.h"
//**************************** start SmoothDrivetrain functions
SmoothDrivetrain::SmoothDrivetrain(MotorPool* motors, uint8_t maxSpeed = 100, bool smooth = true){
    motorPool = motors;
    maxSpeed = maxSpeed;
    smooth = smooth;
}
void SmoothDrivetrain::go(short cmds[], bool smooth){
    motorPool->go(cmds, smooth);
}
void SmoothDrivetrain::stop(){
    short cmd[motorPool->motorsLen];
    for (uint8_t i = 0; i < motorPool->motorsLen; i++){
        cmd[i] = 0;
    }
    SmoothDrivetrain::go(cmd);
}

void SmoothDrivetrain::sync(){
    motorPool->sync();
}
bool SmoothDrivetrain::isCellerating(){
    return motorPool->isCellerating();
}

short SmoothDrivetrain::clampPWM(short input){
    // return a proper range of [-255, 255]
    return (input < -255 ? -255 : (input > 255 ? 255 : input));
}
//**************************** end SmoothDrivetrain functions

//**************************** start Tank functions
Tank::Tank(MotorPool* motors, uint8_t maxSpeed = 100, bool smooth = true) :SmoothDrivetrain(motors, maxSpeed, smooth){
    left = 0;
    right = 0;
}

void Tank::go(short x, short y, bool smooth = true){
    // make sure x and y are in proper range
    x = clampPWM(x);
    y = clampPWM(y);
    // apply speed governor
    if (abs(y) > maxSpeed * 2.55)
        y = maxSpeed * (y <= 0 ? 2.55 : -2.55);
    left = y,  right = y;
    if (y == 0){ // if not going forward or backward (turn on a dime)
        // re-apply speed governor
        if (abs(x) > maxSpeed * 2.55)
            x = maxSpeed * (x <= 0 ? 2.55 : -2.55);
        right = -1 * x;
        left = x;
    }
    else{ // if turning left or right adjust motors' speed accordingly
        if (x > 0)
            right *= (255 - x) / 255.0;
        else if (x < 0)
            left *= ((-255 - x) * -1) / 255.0;
    }
    short cmd[] = {left, right};
    SmoothDrivetrain::go(cmd, smooth);
}
//**************************** end Tank functions

//**************************** start Automotive functions
Automotive::Automotive(MotorPool* motors, uint8_t maxSpeed = 100, bool smooth = true) :SmoothDrivetrain(motors, maxSpeed, smooth){}

void Automotive::go(short x, short y, bool smooth = true){
    // make sure x and y are in proper range
    x = clampPWM(x);
    y = clampPWM(y);
    // apply speed governor
    if (abs(y) > maxSpeed * 2.55)
        y = maxSpeed * (y <= 0 ? 2.55 : -2.55);
    short cmd[] = {x, y};
    SmoothDrivetrain::go(cmd, smooth);
}
//**************************** end Automotive functions

//**************************** start Mecanum functions
Mecanum::Mecanum(MotorPool* motors, uint8_t maxSpeed = 100, bool smooth = true) :SmoothDrivetrain(motors, maxSpeed, smooth){
    left = 0, right = 0;
}

void Mecanum::go(short x, short y, bool strafe, bool smooth = true){
    x = clampPWM(x);
    y = clampPWM(y);
    // apply speed governor
    if (abs(y) > maxSpeed * 2.55)
        y = maxSpeed * (y <= 0 ? 2.55 : -2.55);
    // assuming left/right axis is null (just going forward or backward)
    left = y;
    right = y;
    if (!y){
        // if forward/backward axis is null
        // re-apply speed governor to only axis with a non-zero value
        if (abs(x) > maxSpeed * 2.55)
            x = maxSpeed * (x >= 0 ? 2.55 : -2.55);
        right = x;
        left = x * -1;
        if (!strafe) { // "turning on a dime" functionality
            short cmds[] = {left, left, right, right};
            SmoothDrivetrain::go(cmds, smooth);
        }
        else { // straight strafing functionality
            short cmds[] = {left * -1, left, right, right * -1};
            SmoothDrivetrain::go(cmds, smooth);
        }
    }
    else {
        //  if forward/backward axis is not null and left/right axis is not null
        //  apply differential to motors accordingly
        if (!strafe){
            // veer and propell
            float offset = (255 - abs(x)) / 255.0;
            if (y){
                if (x > 0)
                    right *= offset;
                else if (x < 0)
                    left *= offset;
            }
            short cmds[] = {left, left, right, right};
            SmoothDrivetrain::go(cmds, smooth);
        }
        else{
            // strafe and propell
            if (255 > x && x > 128)
                right *= (255 - abs(x)) / -128;
            else if (0 < x && x <= 128)
                right *= (128 - abs(x)) / 128;
            else if (0 > x && x >= -128)
                left *= (128 - abs(x)) / 128;
            else if (-128 > x && x > -255)
                left *= (255 - abs(x)) / -128;
            if (y > 0){ // if it is strafing right
                short cmds[] = {left, right, left, right};
                SmoothDrivetrain::go(cmds, smooth);
            }
            else{ // if it is strafing left
                short cmds[] = {right, left, right, left};
                SmoothDrivetrain::go(cmds, smooth);
            }
        }
    }
}
//**************************** end Mecanum functions

//**************************** start Locomotive functions
Locomotive::Locomotive(Solenoid* solenoids, uint8_t switchPin){
    isForward = 0, isInMotion = 0;;
    solenoids = solenoids;
    switchPin = switchPin;
    pinMode(switchPin, INPUT);
}

void Locomotive::stop(){
    if (isInMotion)
        isInMotion = false;
}

void Locomotive::go(bool forward){
    isForward = forward;
    isInMotion = true;
    sync();
}

void Locomotive::sync(){
    if (isInMotion){
        int8_t alternate = (digitalRead(switchPin) ? 1 : -1) * (isForward ? -1 : 1);
        solenoids->setValue(alternate);
    }
}

bool Locomotive::isCellerating(){
    return isInMotion;
}
//**************************** end Locomotive functions
