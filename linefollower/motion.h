#ifndef LINEFOLLOWER_MOTION_H_
#define LINEFOLLOWER_MOTION_H_

#include "Arduino.h"

#include "motors.h"
#include "PIDController.h"

class Motion
{
 private:
  Motor motor_lf_;
  Motor motor_rf_;
  Motor motor_lb_;
  Motor motor_rb_;
  PIDController pid_lf_;
  PIDController pid_lb_;
  PIDController pid_rf_;
  PIDController pid_rb_;

  float target_left_velocity_, target_right_velocity_;

  //Private motion functions
  void setVelRaw(bool rf, int pwm_rf, bool lf, int pwm_lf,
                         bool rb, int pwm_rb, bool lb, int pwm_lb);

 public:
  Motion();

  // velocity in m/s, rotational velocity in rad/s
  void setVel(float velocity, float rotational_velocity);

  //Must be called a set rate
  void update();
  
  //STOP THE HORSES
  void stop();
};
 
#endif
