#ifndef MOTORS_H
#define MOTORS_H

class Motor {
  private:
    int pin_1_, pin_2_, pin_pwm_;
    bool forward_state_;
  public:
    Motor(int motor_1_pin, int motor_2_pin, int motor_pwm_pin);
    void Set(float accel, float current_velocity);
	void SetRaw(bool forward, int pwm);
};

extern Motor motor_lf, motor_lb, motor_rf, motor_rb;

#endif
