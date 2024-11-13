#include "Drivetrain.h"

// alias MotorHead namespace
namespace mh = MotorHead;

// declare drivetrain
mh::Tank* drivetrain;

// declare motors
mh::BiMotor* left;
mh::BiMotor* right;

// define PWM control pins for drive motors
#define LEFT_MOTOR_FORWARD 3
#define LEFT_MOTOR_BACKWARD 5
#define RIGHT_MOTOR_FORWARD 6
#define RIGHT_MOTOR_BACKWARD 9


void setup() {
  // instantiate motor objects
  left = new mh::BiMotor(LEFT_MOTOR_FORWARD, LEFT_MOTOR_BACKWARD);
  right = new mh::BiMotor(RIGHT_MOTOR_FORWARD, RIGHT_MOTOR_BACKWARD);
  // instantiate drivetrain object (motors' speeds default to 0)
  drivetrain = new mh::Tank(left, right);
  Serial.begin(115200);  // open a channel to pour data into & get commands
}

void loop() {
  drivetrain->sync();
  while (Serial.available() > 2) {  //read from buffer if more than 2 bytes
    // LF & CR (2 bytes) seem to linger in stream buffer for 1 extra loop() iteration
    
    // NOTE: stream expected format  = "# # #" where # == [-100,100]
    // delimiter can be any non-digit character (example above uses ' ')
    // use x for left-right steering
    // use y for forward-backward propulsion
    short x = round(Serial.parseInt() * 2.55);
    short y = round(Serial.parseInt() * 2.55);
    drivetrain->go(x, y);
  }
}
