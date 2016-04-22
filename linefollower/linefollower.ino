#include <EncoderMod.h>
#include <Servo.h>

#include "conf.h"
#include "PIDController.h"
#include "motion.h"
#include "SensorBar.h"

int sensor_bar_pins[] = {IR2, IR3, IR4, IR5, IR6,
                         IR7, IR8, IR9, IR10 , IR11};

int sensor_bar_weights[] = {-5, -4, -3, -2, -1,
                            1, 2, 3, 4, 5};

Servo servo;
PIDController servo_pid (SERVO_KP, SERVO_KI, SERVO_KD, SERVO_INTEGRAL_CAP);

// 'l' for left, 'r' for right, 's' for straight
char servo_turn_direction = 's';

Motion motion;
SensorBar sensor_bar(sensor_bar_pins, sensor_bar_weights, 600);

void setup()
{
  Serial.begin(9600);

  // Set higher pwm frequency for smoother motor control.
  analogWriteFrequency(MOTOR_LF_PWM, 46875);
  analogWriteFrequency(MOTOR_RF_PWM, 46875);
  analogWriteFrequency(MOTOR_LB_PWM, 46875);
  analogWriteFrequency(MOTOR_RB_PWM, 46875);

  // PWM resolution is 0-1023.
  analogWriteResolution(10);

  // set up servo
  servo.attach(SERVO_PIN);
  servo.write(90);
 

  for (int i = 0; i < SENSOR_BAR_LEN; i++) {
    pinMode(sensor_bar_pins[i], INPUT);
  }

  pinMode(LEFT_SIDE_SENSOR_PIN, INPUT);
  pinMode(RIGHT_SIDE_SENSOR_PIN, INPUT);

  pinMode(MOTOR_LF_1, OUTPUT);
  pinMode(MOTOR_LF_2, OUTPUT);
  pinMode(MOTOR_LF_PWM, OUTPUT);
  pinMode(MOTOR_RF_1, OUTPUT);
  pinMode(MOTOR_RF_2, OUTPUT);
  pinMode(MOTOR_RF_PWM, OUTPUT);
  pinMode(MOTOR_LB_1, OUTPUT);
  pinMode(MOTOR_LB_2, OUTPUT);
  pinMode(MOTOR_LB_PWM, OUTPUT);
  pinMode(MOTOR_RB_1, OUTPUT);
  pinMode(MOTOR_RB_2, OUTPUT);
  pinMode(MOTOR_RB_PWM, OUTPUT);

  // set up onboard led
  pinMode(13, OUTPUT);
  digitalWrite(13, 0);

  //Setup the pushbutton pin
  pinMode(BUTTON_PIN, INPUT);

  //Delay until the button is pressed
  delay(1000);
  while(digitalRead(BUTTON_PIN) == 0);
  delay(500);

  // log initial sensor values
  for (int i = 0; i < SENSOR_BAR_LEN; i++) {
    Serial.print(analogRead(sensor_bar_pins[i]));
    Serial.print("\t");
  }

  if (analogRead(28)*8.33/791 < 7.3) {
    while (1) {
      digitalWrite(13, 1);
      delay(100);
      digitalWrite(13, 0);
      delay(300);
    }
  }
}

PIDController turn_pid (KP_TURN, KI_TURN, KD_TURN);
void loop()
{
  float error = sensor_bar.readAngle();

  if (isnan(error)) {
    if (servo_turn_direction == 'r') {
      error = SERVO_MAX_ERROR;
    } else {
      error = -SERVO_MAX_ERROR;
    }
  }

  //int servo_position = servo_pid.Calculate(error, 0);
  //if (servo_position > 75) {
  //  servo_position = 75;
  //} else if (servo_position < -75) {
  //  servo_position = -75;
  //}

  if (error > 0) {
    servo_turn_direction = 'r';
  } else if (error == 0) {
    servo_turn_direction = 's';
  } else {
    servo_turn_direction = 'l';
  }

//  servo.write(servo_position + 90);
  float rotation = turn_pid.Calculate(-error, 0);
  float forward_vel = MAX_FORWARD_SPEED - 0.030*abs(rotation);
  motion.setVel(forward_vel, rotation);
  motion.update();
  
  if (digitalRead(BUTTON_PIN) == 1) {
	motion.stop();
	delay(3000);
    while(digitalRead(BUTTON_PIN) == 0) {
      motion.stop();
    }
	delay(1000);
    if (analogRead(28)*8.33/791 < 7.3) {
      while (1) {
        digitalWrite(13, 1);
        delay(100);
        digitalWrite(13, 0);
        delay(300);
      }
    }
  }
}
