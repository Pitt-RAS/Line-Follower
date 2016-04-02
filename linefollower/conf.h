#ifndef LINEFOLLOWER_CONF_H_
#define LINEFOLLOWER_CONF_H_

#include <Arduino.h>

// time between interrupt calls (in us)
#define DT 1000

// pin numbers
#define MOTOR_LF_1 2
#define MOTOR_LF_2 3
#define MOTOR_LF_PWM 5
#define ENC_LF_1 33
#define ENC_LF_2 32

#define MOTOR_LB_1 4
#define MOTOR_LB_2 7
#define MOTOR_LB_PWM 6
#define ENC_LB_1 31
#define ENC_LB_2 30

#define MOTOR_RF_1 8
#define MOTOR_RF_2 11
#define MOTOR_RF_PWM 9
#define ENC_RF_1 16
#define ENC_RF_2 17

#define MOTOR_RB_1 12
#define MOTOR_RB_2 13
#define MOTOR_RB_PWM 10
#define ENC_RB_1 14
#define ENC_RB_2 15

#define IR0 A15
#define IR1 A16

#define IR2 A13
#define IR3 A12
#define IR4 A11
#define IR5 A10
#define IR6 A4
#define IR7 A5
#define IR8 A6
#define IR9 A7
#define IR10 A8
#define IR11 A9
 
#define LEFT_SIDE_SENSOR_PIN IR0
#define RIGHT_SIDE_SENSOR_PIN IR11

#define SERVO_PIN 25

// robot characteristics
#define ROBOT_MASS 0.229 // kg
#define NUMBER_OF_MOTORS 4
#define STEPS_PER_MOTOR_REV 12// the number of encoder steps we get per wheel revolution
#define MM_PER_STEP 0.653868
#define SENSOR_BAR_LEN 10
#define FORCE_PER_AMP
#define VELOCITY_PER_BEMF
#define RATED_INTERNAL_RESISTANCE
#define BATTERY_VOLTAGE 8.1
#define GEAR_RATIO 9.96 // gear ratio between motor and wheels
#define PWM_SPEED_STEPS 1023 // maximum PWM value for the system

// Motor spec sheet parameters
#define RATED_VOLTAGE 8 // Voltage that ratings were measured with
#define RATED_INTERNAL_RESISTANCE 3.1 // ohms of resistance in motor coils
#define RATED_FREERUN_CURRENT 0.06// Amps
#define RATED_RPM_PER_VBEMF 818000 // RPM/Volt
#define RATED_TORQUE_PER_AMP 0.000622353 // torque in N-m at output shaft, Amps passed through motor

// Convert motor parameters to robot parameters
#define WHEEL_RADIUS (MM_PER_STEP * STEPS_PER_MOTOR_REV * GEAR_RATIO / (2 * 3.14159265359))// in mm
#define FORCE_PER_AMP (RATED_TORQUE_PER_AMP * GEAR_RATIO * NUMBER_OF_MOTORS * 1000 / WHEEL_RADIUS) // in N
#define VELOCITY_PER_VBEMF (RATED_RPM_PER_VBEMF * STEPS_PER_MOTOR_REV * GEAR_RATIO * MM_PER_STEP / (60000)) // in m/s/V
#define FRICTION_FORCE (RATED_FREERUN_CURRENT * FORCE_PER_AMP + 0.10) // Newtons (0.08 calculated Newtons from motor/gearbox)  amount of force opposing motion in robot including rolling resistance, sliding, gearing

// control tuning
#define MOTOR_KP 2
#define MAX_STRAIGHT_PWM 80
#define MAX_TURN_PWM 135

#define SERVO_KP 0.04
#define SERVO_HOLD_WIDTH 1
#define SERVO_KI 0
#define SERVO_KD 0.0009
#define SERVO_INTEGRAL_CAP 0

#endif
