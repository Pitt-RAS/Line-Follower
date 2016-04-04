#include <Arduino.h>
#include "conf.h"
#include "motors.h"

Motor motor_lf (MOTOR_LF_1, MOTOR_LF_2, MOTOR_LF_PWM);
Motor motor_rf (MOTOR_RF_1, MOTOR_RF_2, MOTOR_RF_PWM);
Motor motor_lb (MOTOR_LB_1, MOTOR_LB_2, MOTOR_LB_PWM);
Motor motor_rb (MOTOR_RB_1, MOTOR_RB_2, MOTOR_RB_PWM);

Motor::Motor(int motor_1_pin, int motor_2_pin, int motor_pwm_pin) {
  pin_1_ = motor_1_pin;
  pin_2_ = motor_2_pin;
  pin_pwm_ = motor_pwm_pin;

  pinMode(pin_1_, OUTPUT);
  pinMode(pin_2_, OUTPUT);
  pinMode(pin_pwm_, OUTPUT);
}

void Motor::Set(float accel, float current_velocity) {
  float force;
  int pin_state;
  int speed_raw;

  if (current_velocity > 0) {
    force = (ROBOT_MASS * accel + FRICTION_FORCE) / NUMBER_OF_MOTORS;
  } else if (current_velocity < 0) {
    force = (ROBOT_MASS * accel - FRICTION_FORCE) / NUMBER_OF_MOTORS;
  } else {
    force = (ROBOT_MASS * accel) / NUMBER_OF_MOTORS;
  }
  float required_current = force / FORCE_PER_AMP;
  float back_emf = current_velocity / VELOCITY_PER_VBEMF;
  float speed = ((required_current * RATED_INTERNAL_RESISTANCE + back_emf) / BATTERY_VOLTAGE);
  speed = constrain(speed, -1, 1);

  speed_raw = abs((int)(round(PWM_SPEED_STEPS * speed)));

  if (speed > 0.0) {
    pin_state = HIGH;
  } else {
    pin_state = LOW;
  }
  Serial.print(current_velocity);
  Serial.print("   ");
  Serial.println(speed_raw * BATTERY_VOLTAGE /1024.0);
  digitalWrite(pin_1_, pin_state);
  digitalWrite(pin_2_, pin_state ^ 1);
  analogWrite(pin_pwm_, speed_raw);
}
