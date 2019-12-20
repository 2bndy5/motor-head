#ifndef Motors_h
#define Motors_h
#include <arduino.h>

class SmoothMotor{
public:
    SmoothMotor(int rampTime);
    short cellerate(short s);
    bool isCellerating();
    void sync();
protected:
    virtual void write(short){};
    short value;
    unsigned int rampTime;
    short targetSpeed, initSpeed;
    unsigned long initSmooth, endSmooth;
};

class Solenoid{
public:
    Solenoid(uint8_t plusPin, uint8_t negPin = NULL);
    virtual short getValue();
    void setValue(short s);
    // the rest of these public functions ensure drop-in replacement of any motor with a solenoid
    short cellerate(short s){ setValue(s); };
    bool isCellerating(){ return false; };
    void sync(){ return; };
protected:
    virtual void write(short);
    unsigned char pin1, pin2, signal1, signal2;
private:
    short value;
};

class BiMotor: public SmoothMotor, public Solenoid {
public:
    BiMotor(uint8_t plusPin, uint8_t negPin, int dt = 2000);
    short getValue();
private:
    void write(short);
};

class PhasedMotor: public SmoothMotor, public Solenoid {
public:
    PhasedMotor(uint8_t phasedPin, uint8_t pwmPin, int dt = 2000);
    short getValue();
private:
    void write(short);
};

class MotorPool{
public:
    MotorPool(Solenoid* motors[]);
    void go(int cmds[], bool smooth = true);
    bool isCellerating();
    void sync();
    // bool insert(uint8_t index, Solenoid obj);
    // bool remove(Solenoid obj);
    // bool append(Solenoid obj);
    // Solenoid pop(uint8_t index = NULL);
    uint8_t motorsLen;
protected:
    Solenoid* motors[];
};

#endif
