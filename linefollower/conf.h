#ifndef LINEFOLLOWER_CONF_H_
#define LINEFOLLOWER_CONF_H_

#include <Arduino.h>

// pin numbers
#define MOTOR_LB_1 3
#define MOTOR_LB_2 2
#define MOTOR_LB_PWM 5
#define ENC_LB_1 33
#define ENC_LB_2 32

#define MOTOR_LF_1 7
#define MOTOR_LF_2 4
#define MOTOR_LF_PWM 6
#define ENC_LF_1 31
#define ENC_LF_2 30

#define MOTOR_RF_1 8
#define MOTOR_RF_2 11
#define MOTOR_RF_PWM 9
#define ENC_RF_1 17
#define ENC_RF_2 16

#define MOTOR_RB_1 13
#define MOTOR_RB_2 12
#define MOTOR_RB_PWM 10
#define ENC_RB_1 15
#define ENC_RB_2 14

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

#define BUTTON_PIN 29

// robot characteristics
#define NUM_SENSORS 10

#define ROBOT_MASS 0.229 // kg
#define NUMBER_OF_MOTORS 4
#define STEPS_PER_MOTOR_REV 12// the number of encoder steps we get per wheel revolution
#define MM_PER_STEP 0.788552
#define SENSOR_BAR_LEN 10
#define BATTERY_VOLTAGE 8.1
#define GEAR_RATIO 9.96 // gear ratio between motor and wheels
#define PWM_SPEED_STEPS 1023 // maximum PWM value for the system

// Motor spec sheet parameters
#define RATED_VOLTAGE 8 // Voltage that ratings were measured with
#define RATED_INTERNAL_RESISTANCE 3.1 // ohms of resistance in motor coils
#define RATED_FREERUN_CURRENT 0.06// Amps
#define RATED_RPM_PER_VBEMF (29980.0/6.0)//818000 // RPM/Volt
#define RATED_TORQUE_PER_AMP 0.000622353 // torque in N-m at output shaft, Amps passed through motor
#define RATED_OUTPUT_SHAFT_RPS_PER_VBEMF (RATED_RPM_PER_VBEMF/60.0/GEAR_RATIO)

// Convert motor parameters to robot parameters
#define WHEEL_RADIUS (MM_PER_STEP * STEPS_PER_MOTOR_REV * GEAR_RATIO / (2 * 3.14159265359))// in mm
#define FORCE_PER_AMP (RATED_TORQUE_PER_AMP * GEAR_RATIO * (1000 / WHEEL_RADIUS)) // in N/Amps
#define VELOCITY_PER_VBEMF (RATED_OUTPUT_SHAFT_RPS_PER_VBEMF * (0.03 * 3.14159265359))//(//(RATED_RPM_PER_VBEMF * STEPS_PER_MOTOR_REV * GEAR_RATIO * MM_PER_STEP / (60000)) // in m/s/V
#define FRICTION_FORCE (RATED_FREERUN_CURRENT * FORCE_PER_AMP + 0.10) // Newtons (0.08 calculated Newtons from motor/gearbox)  amount of force opposing motion in robot including rolling resistance, sliding, gearing

// control tuning
#define MAX_FORWARD_SPEED 1

#define SERVO_KP 0
#define SERVO_KI 4
#define SERVO_KD 0
#define SERVO_INTEGRAL_CAP 1000
#define SERVO_MAX_ERROR 6

#define KP_VELOCITY 10
#define KI_VELOCITY 0
#define KD_VELOCITY 0
#define VELOCITY_INTEGRAL_CAP 10000

#define DISTANCE_BETWEEN_WHEELS (108.0/1000.0) //108mm

#endif
