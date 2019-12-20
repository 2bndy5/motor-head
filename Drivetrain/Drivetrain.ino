#include "Drivetrain.h"

// declare drivetrain
SmoothDrivetrain* d;
// define PWM control pins for drive motors
BiMotor* M1 = new BiMotor(3, 5);
BiMotor* M2 = new BiMotor(6, 9);
// declare the motor pool for the drivetrain
Solenoid* motorList[] = {M1, M2};
MotorPool mPool = MotorPool(motorList);

void setup(){
  Serial.begin(115200);// open a channel to pour data into & get commands
  // instantiate drivetrain object (motors' speeds default to 0)
  d = new Automotive(&mPool);
}

void loop(){
  d->sync();
  while (Serial.available() > 2){ //read from buffer if more than 2 bytes
    // LF & CR (2 bytes) seem to linger in stream buffer for 1 extra loop() iteration
    // stream expected format  = "# # #" where # == [-100,100]
    // delimiter can be any non-digit character (example above uses ' ')
    // use x for left-right steering
    // use y for forward-backward drive
    short x = round(Serial.parseInt() * 2.55);
    short y = round(Serial.parseInt() * 2.55);
    // short z = Serial.parseInt();
    short cmds[] = {x, y};
    d->go(cmds);

  }
}
