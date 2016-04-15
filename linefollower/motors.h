#ifndef MOTORS_H
#define MOTORS_H

#include <EncoderMod.h>

class Motor {
 private:
  int pin_1_, pin_2_, pin_pwm_;
  bool forward_state_;
  Encoder encoder_;
 public:
  Motor(int motor_1_pin, int motor_2_pin, int motor_pwm_pin,
        int encoder_1_pin, int encoder_2_pin);
  void set(float accel, float current_velocity);
  void setRaw(bool forward, int pwm);

  // position is in mm
  float getPosition();
  void setPosition(float new_position);

  // velocity is in m/s
  float getVelocity();
};

#endif
