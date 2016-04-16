#include "motion.h"

#include "conf.h"

Motion::Motion()
    : motor_lf_ (MOTOR_LF_1, MOTOR_LF_2, MOTOR_LF_PWM, ENC_LF_1, ENC_LF_2),
      motor_rf_ (MOTOR_RF_1, MOTOR_RF_2, MOTOR_RF_PWM, ENC_RF_1, ENC_RF_2),
      motor_lb_ (MOTOR_LB_1, MOTOR_LB_2, MOTOR_LB_PWM, ENC_LB_1, ENC_LB_2),
      motor_rb_ (MOTOR_RB_1, MOTOR_RB_2, MOTOR_RB_PWM, ENC_RB_1, ENC_RB_2),
      pid_lf_ (KP_VELOCITY, KI_VELOCITY, KD_VELOCITY, VELOCITY_INTEGRAL_CAP),
      pid_lb_ (KP_VELOCITY, KI_VELOCITY, KD_VELOCITY, VELOCITY_INTEGRAL_CAP),
      pid_rf_ (KP_VELOCITY, KI_VELOCITY, KD_VELOCITY, VELOCITY_INTEGRAL_CAP),
      pid_rb_ (KP_VELOCITY, KI_VELOCITY, KD_VELOCITY, VELOCITY_INTEGRAL_CAP),
      target_left_velocity_(0), target_right_velocity_(0)
{
  // Setup the interrupt to call update
}

void Motion::update()
{
  // Update PID controllers
  // Compare current velocities to desired, using PID loop to calculate the new the settings to send to setVelRaw();
  float accel_lf = pid_lf_.Calculate(motor_lf_.getVelocity(), target_left_velocity_);
  float accel_lb = pid_lb_.Calculate(motor_lb_.getVelocity(), target_left_velocity_);
  float accel_rf = pid_rf_.Calculate(motor_rf_.getVelocity(), target_right_velocity_);
  float accel_rb = pid_rb_.Calculate(motor_rb_.getVelocity(), target_right_velocity_);

  motor_lf_.set(accel_lf, motor_lf_.getVelocity());
  motor_lb_.set(accel_lb, motor_lb_.getVelocity());
  motor_rf_.set(accel_rf, motor_rf_.getVelocity());
  motor_rb_.set(accel_rb, motor_rb_.getVelocity());
}

void Motion::setVel(float velocity, float rotational_velocity)
{
  float rotation_offset = rotational_velocity * DISTANCE_BETWEEN_WHEELS / 2;
  target_left_velocity_ = velocity + rotation_offset;
  target_right_velocity_ = velocity - rotation_offset;
}

void Motion::stop()
{
  setVelRaw(true, 0, true, 0, true, 0, true, 0);
}

// Private methods
void Motion::setVelRaw(bool rf, int pwm_rf, bool lf, int pwm_lf,
                       bool rb, int pwm_rb, bool lb, int pwm_lb)
{
  motor_rf_.set(rf, pwm_rf);
  motor_rb_.set(rb, pwm_rb);
  motor_lb_.set(lb, pwm_lb);
  motor_lf_.set(lf, pwm_lf);
}
