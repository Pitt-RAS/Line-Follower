#include <EncoderMod.h>
#include <Servo.h>

#include "conf.h"
#include "motors.h"
#include "PIDController.h"
#include "motion.h"
#include "SensorBar.h"

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

int sensor_bar_pins[] = {IR2, IR3, IR4, IR5, IR6,
                         IR7, IR8, IR9, IR10 , IR11};

int sensor_bar_weights[] = {-5, -4, -3, -2, -1,
							1, 2, 3, 4, 5};
Servo servo;
int servo_position = 0;
int servo_integral = 0;
int last_center = 0;

int analog_cutoff = 150;

uint16_t sensor_values = 0;

// 'l' for left, 'r' for right, 's' for straight
char servo_turn_direction = 's';

char serial_buf[600];
volatile bool serial_buf_ready = false;
int serial_chars = 0;

IntervalTimer pid_timer;
Motion motion;
SensorBar sensor_bar(sensor_bar_pins, sensor_bar_weights, 500);

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
  //pinMode(BUTTON_PIN, INPUT);

  //Delay until the button is pressed
  delay(1000);
  //while(digitalRead(BUTTON_PIN) == 1);
  //delay(500);

  // log initial sensor values
  for (int i = 0; i < SENSOR_BAR_LEN; i++) {
    Serial.print(analogRead(sensor_bar_pins[i]));
    Serial.print("\t");
  }
  //while(1){
	//Serial.println(sensor_bar.readAngle());
	//delay(100);
  //}
  
  //while (1) {}

  // start timed pid loop interrupt
//  pid_timer.begin(pid_interrupt, DT);
}

void loop() {
  float error = sensor_bar.readAngle();
  if(error != PROXIMITY_INACTIVE){
	motion.setVel(0, 4.0*error);
  }
  else {
	  motion.setVel(0, 0);
  }
  delay(100);
}
