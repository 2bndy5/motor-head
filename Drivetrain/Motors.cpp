#include "Motors.h"
#include <arduino.h>

MotorPool::MotorPool(Solenoid* motors[]){
        motors = motors;
        motorsLen = sizeof(motors) / sizeof(motors[0]);
}

void MotorPool::go(int cmds[], bool smooth = true){
    for (uint8_t i; i < (sizeof(cmds) / sizeof(cmds[0])); i++){
        if (i < motorsLen){
            if (smooth) { // if input is getting smoothed
                motors[i]->cellerate(cmds);
            }
            else{
                motors[i]->setValue(cmds);
            }
        }
    }
    // i = 0
    // while i < len(self._motors):
    //     skip = len(self._motors[i])
    //     for j in range(skip):
    //         if smooth:
    //             self._motors[i + j].cellerate(cmds[i + j])
    //         else:
    //             self._motors[i + j].value = cmds[i + j]
    //     i += skip
}

bool MotorPool::isCellerating(){
    for (uint8_t i = 0; i < motorsLen; i++){
        if (motors[i]->isCellerating())
            return true;
    }
    return false;
}

void MotorPool::sync(){
    for (uint8_t i = 0; i < motorsLen; i++)
        motors[i]->sync();
}
/* // for motors array management
bool MotorPool::insert(uint8_t index, Solenoid obj){
    motors.insert(index, obj);
}

bool MotorPool::remove(Solenoid obj){
    motors.remove(obj);
}

bool MotorPool::append(Solenoid obj){
    motors.append(obj);
}

Solenoid MotorPool::pop(uint8_t index = NULL){
    if (index < motorsLen)
        return motors.pop(index);
    else{
        return motors.pop();
    }
}
*/
// ******************************** end functions for MotorPool

// ******************************** start functions for SmoothMotor
SmoothMotor::SmoothMotor(int dt){
    rampTime = dt, value = 0;
    targetSpeed = 0, initSpeed = 0;
    initSmooth = 0, endSmooth = 0;
}

bool SmoothMotor::isCellerating(){
    return value != targetSpeed;
}

short SmoothMotor::cellerate(short s){
    targetSpeed = max(-255, min(255, s));
    initSmooth = millis();
    initSpeed = value;
    double of_dt = abs((double)(targetSpeed - initSpeed) / 510.0);
    endSmooth = initSmooth + of_dt * rampTime;
    sync();
}

void SmoothMotor::sync(){
    unsigned long now = millis();
    if (endSmooth != initSmooth && now > endSmooth && rampTime){
        double ds = (1 - cos(now / (double)(endSmooth - initSmooth) * PI)) / 2;
        write((short)(ds * (targetSpeed - initSpeed) + initSpeed));
    }
    else{
        write(targetSpeed);
    }
}
// ******************************** end functions for SmoothMotor

// ******************************** start functions for Solenoid
Solenoid::Solenoid(uint8_t plusPin, uint8_t negPin = NULL){
    // save pin #s and set pin mode
    pinMode(plusPin, OUTPUT);
    pin1 = plusPin;
    if (negPin != NULL){
        pinMode(negPin, OUTPUT);
        pin2 = negPin;
    }
    else pin2 = NULL;
    signal1 = 0, signal2 = 0, value = 0;
}

void Solenoid::setValue(short s){
    s = max(-255, min(255, s));
    write(s);
}

short Solenoid::getValue(){
    if (pin2 != NULL){
        return (short)((bool)signal1 - (bool)signal2);
    }
    else{
       return (short)((bool)signal1);
    }
}

void Solenoid::write(short s){

    if (s < 0){
        signal1 = 0; signal2 = abs(s);
    }
    else if (s >= 0){
        signal1 = s, signal2 = 0;
    }
    digitalWrite(pin1, signal1);
    if (pin2 != NULL){
        digitalWrite(pin2, signal2);
    }
    // Serial.print(s);
    // Serial.println(" = Speed wrote with Solenoid Parent class");
    value = getValue();
}
// ******************************** end functions for Solenoid

// ******************************** start functions for BiMotor
BiMotor::BiMotor(uint8_t plusPin, uint8_t negPin = NULL, int dt): SmoothMotor(dt), Solenoid(plusPin, negPin){}

void BiMotor::write(short s){
    if (s < 0){ // if going backward
        signal1 = 0;
        signal2 = -1 * s;
    }
    else if (s >= 0){// if going forward or stopped
        signal1 = s;
        signal2 = 0;
    }
    analogWrite(pin1, signal1);
    if (pin2 != NULL)
        analogWrite(pin2, signal2);
    // Serial.print("motor pwm: ");Serial.println(s);
    SmoothMotor::value = getValue();
}

short BiMotor::getValue(){
    if (pin2 != NULL){
        return (short)signal1 - signal2;
    }
    else{
       return (short)signal1;
    }
}
// ******************************** end functions for BiMotor

// ******************************** start functions for PhasedMotor
PhasedMotor::PhasedMotor(uint8_t phasedPin, uint8_t pwmPin, int dt): SmoothMotor(dt), Solenoid(phasedPin, pwmPin){}

void PhasedMotor::write(short s){
    if (s <= 0){     // if going backward or stopped
        signal1 = false;
        signal2 = -1 * s;
    }
    else if (s > 0){// if going forward
        signal1 = true;
        signal2 = s;
    }
    digitalWrite(pin1, signal1);
    if (pin2 != NULL)
        analogWrite(pin2, signal2);
    // Serial.print("motor pwm: ");Serial.println(s);
    SmoothMotor::value = getValue();
}

short PhasedMotor::getValue(){
    if (pin2 != NULL){
        return (short)signal2 - (!signal1 * 255);
    }
    else{
       return (short)signal1 * 255;
    }
}
// ******************************** end functions for BiMotor
