#include <Arduino.h>
#include "conf.h"
#include "motors.h"

Motor::Motor(int motor_1_pin, int motor_2_pin, int motor_pwm_pin,
             int encoder_1_pin, int encoder_2_pin)
    : pin_1_(motor_1_pin), pin_2_(motor_2_pin), pin_pwm_(motor_pwm_pin),
      encoder_(encoder_1_pin, encoder_2_pin) {
  pinMode(pin_1_, OUTPUT);
  pinMode(pin_2_, OUTPUT);
  pinMode(pin_pwm_, OUTPUT);
}

void Motor::set(float accel, float current_velocity) {
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

  digitalWrite(pin_1_, pin_state);
  digitalWrite(pin_2_, pin_state ^ 1);
  analogWrite(pin_pwm_, speed_raw);
}

void Motor::setRaw(bool forward, int speed_raw)
{
  if (forward) {
    digitalWrite(pin_1_, HIGH);
    digitalWrite(pin_2_, LOW);
  } else {
    digitalWrite(pin_1_, LOW);
    digitalWrite(pin_2_, HIGH);
  }
  analogWrite(pin_pwm_, speed_raw);
}

float Motor::getPosition()
{
  return encoder_.extrapolate() * MM_PER_STEP;
}

void Motor::setPosition(float new_position)
{
  encoder_.write((int32_t)rint(new_position / MM_PER_STEP));
}

float Motor::getVelocity()
{
  return encoder_.stepRate() * 1000 * MM_PER_STEP;
}
