#include <EncoderMod.h>
#include <Servo.h>

#include "conf.h"
#include "motors.h"

Encoder encoderRF(ENC_RF_1, ENC_RF_2);
Encoder encoderLF(ENC_LF_1, ENC_LF_2);
Encoder encoderRB(ENC_RB_1, ENC_RB_2); 
Encoder encoderLB(ENC_LB_1, ENC_LB_2);

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

int sensor_bar_pins[] = {IR1, IR2, IR3, IR4, IR5,
                         IR6, IR7, IR8, IR9, IR10};

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
  Serial.println();

  uint32_t t = millis();

  //while(1){
  //  Serial.println(-encoderRB.stepRate() * 1000000 * MM_PER_STEP / 1000);
  //}
  
  while (millis() - t < 100) {
    motor_lf.Set(2.0, encoderLF.stepRate() * 1000000 * MM_PER_STEP / 1000);
    motor_rf.Set(2.0, encoderRF.stepRate() * 1000000 * MM_PER_STEP / 1000);
    motor_lb.Set(2.0, encoderLB.stepRate() * 1000000 * MM_PER_STEP / 1000);
    motor_rb.Set(2.0, encoderRB.stepRate() * 1000000 * MM_PER_STEP / 1000);
    delay(10);
  }
  motor_rb.Set(0.0, encoderRB.stepRate() * 1000000 * MM_PER_STEP / 1000);

  t = millis();
  while (millis() - t < 100) {
    motor_lf.Set(-2.0, encoderLF.stepRate() * 1000000 * MM_PER_STEP / 1000);
    motor_rf.Set(-2.0, encoderRF.stepRate() * 1000000 * MM_PER_STEP / 1000);
    motor_lb.Set(-2.0, encoderLB.stepRate() * 1000000 * MM_PER_STEP / 1000);
    motor_rb.Set(-2.0, encoderRB.stepRate() * 1000000 * MM_PER_STEP / 1000);
    delay(10);
  }
  t = millis();
  while (millis() - t < 1000) {
    Serial.println(encoderRB.stepRate() * 1000000 * MM_PER_STEP / 1000);
    delay(50);
  }

  while(1);
  
  t = millis();
  while (millis() - t < 1000) {
    motor_lf.Set(-0.5, 0);
    motor_rf.Set(-0.5, 0);
    motor_lb.Set(-0.5, 0);
    motor_rb.Set(-0.5, 0);
  }

  t = millis();
  while (millis() - t < 1000) {
    motor_lf.Set(-0.5, 0);
    motor_rf.Set(0.5, 0);
    motor_lb.Set(-0.5, 0);
    motor_rb.Set(0.5, 0);
  }

  t = millis();
  while (millis() - t < 1000) {
    motor_lf.Set(0.5, 0);
    motor_rf.Set(-0.5, 0);
    motor_lb.Set(0.5, 0);
    motor_rb.Set(-0.5, 0);
  }

  motor_lf.Set(0.5, 0);
  motor_rf.Set(-0.5, 0);
  motor_lb.Set(0.5, 0);
  motor_rb.Set(-0.5, 0);
  
  while (1) {}

  // start timed pid loop interrupt
//  pid_timer.begin(pid_interrupt, DT);
}

void loop() {
  while (!serial_buf_ready) {}

  int serial_chars_printed = 0;

  while (serial_chars_printed < serial_chars) {
    for (int i = 0; serial_chars_printed < serial_chars && i < 300; i++) {
      Serial.write(serial_buf[serial_chars_printed]);
      serial_chars_printed++;
    }
    delay(30);
  }

  Serial.write('\r');
  Serial.write('\n');
  serial_chars = 0;
  serial_buf_ready = false;
}

uint32_t t = 0;
void pid_interrupt()
{
  bool print_locked = !serial_buf_ready;

  if (micros() - t > DT + 10 && t != 0) digitalWrite(13, 1);
  if (print_locked) serial_chars += sprintf(serial_buf + serial_chars, "%lu\t", micros() - t);
  t = micros();

  // get photosensor data
  sensor_values = 0;
  for (int i = 0; i < SENSOR_BAR_LEN; i++) {
    sensor_values |= (analogRead(sensor_bar_pins[i]) < analog_cutoff) << i;
  }

  // log photosensor data
  for (int i = 0; i < SENSOR_BAR_LEN; i++) {
    if (print_locked) serial_chars += sprintf(serial_buf + serial_chars, "%i", (sensor_values >> i) & 1);
  }
  if (print_locked) serial_chars += sprintf(serial_buf + serial_chars, "\t");

  // find center of line in photosensor data
  int left_side_of_line = 0;
  while (sensor_values & _BV(left_side_of_line) && left_side_of_line < SENSOR_BAR_LEN) {
    left_side_of_line++;
  }

  int right_side_of_line = SENSOR_BAR_LEN - 1;
  while (sensor_values & _BV(right_side_of_line) && right_side_of_line >= 0) {
    right_side_of_line--;
  }

  // this is in units of half-sensors
  // e.g. if the center of the line is 2 sensors away from center, center would be 4
  int center;

  if (left_side_of_line > right_side_of_line) {
    // entire view is white
    // turn in the same direction we were turning before
    switch (servo_turn_direction) {
      case 'l':
        center = -SENSOR_BAR_LEN;
        break;
      case 'r':
      default:
        center = SENSOR_BAR_LEN;
        break;
    }
  } else if (left_side_of_line == 0 && right_side_of_line == SENSOR_BAR_LEN - 1) {
    // entire view is black, send sensor back to center
    if (servo_position < 0) {
      center = SENSOR_BAR_LEN;
    } else {
      center = -SENSOR_BAR_LEN;
    }
  } else {
    // line is in view
    center = left_side_of_line + right_side_of_line - SENSOR_BAR_LEN + 1; 
  }

  // log center data
  if (print_locked) {
    serial_chars += sprintf(serial_buf + serial_chars, "%i\t", left_side_of_line);
    serial_chars += sprintf(serial_buf + serial_chars, "%i\t", right_side_of_line);
    serial_chars += sprintf(serial_buf + serial_chars, "%f\t", center);
  }

  // servo PID controller
//  servo_integral += SERVO_KI * center * DT / 1000000.0;
//  servo_integral = constrain(servo_integral, -SERVO_INTEGRAL_CAP, SERVO_INTEGRAL_CAP);
//  servo_position += SERVO_KP * center + servo_integral + SERVO_KD * (center - last_center) / (DT / 1000000.0);
//  last_center = center;
  if (abs(center) <= SERVO_HOLD_WIDTH) center = 0;
  servo_position = constrain(servo_position + SERVO_KP * center, -90, 90);

  // write out servo position
  servo.write(servo_position + 90);

  // save servo turn direction
  if (center > 0) {
    servo_turn_direction = 'r';
  } else if (center == 0) {
    servo_turn_direction = 's';
  } else {
    servo_turn_direction = 'l';
  }

  if (print_locked) serial_chars += sprintf(serial_buf + serial_chars, "%f\t", servo_position);

  // motor PID controller
  float left_motor_out = constrain(MAX_STRAIGHT_PWM + MOTOR_KP*(servo_position + 3 * center), -MAX_TURN_PWM, MAX_TURN_PWM);
  float right_motor_out = constrain(MAX_STRAIGHT_PWM - MOTOR_KP*(servo_position + 3 * center), -MAX_TURN_PWM, MAX_TURN_PWM);

  // write speeds to motors
  //if (left_motor_out > 0) {
  //  digitalWrite(MOTORL_1_PIN, 0);
  //  digitalWrite(MOTORL_2_PIN, 1);
  //} else {
  //  digitalWrite(MOTORL_1_PIN, 1);
  //  digitalWrite(MOTORL_2_PIN, 0);
  //}

  //if (right_motor_out > 0) {
  //  digitalWrite(MOTORR_1_PIN, 0);
  //  digitalWrite(MOTORR_2_PIN, 1);
  //} else {
  //  digitalWrite(MOTORR_1_PIN, 1);
  //  digitalWrite(MOTORR_2_PIN, 0);
  //}

  //analogWrite(MOTORL_PWM_PIN, abs(left_motor_out));
  //analogWrite(MOTORR_PWM_PIN, abs(right_motor_out));

  // log motor speeds
  if (print_locked) serial_chars += sprintf(serial_buf + serial_chars, "%f\t", left_motor_out);
  if (print_locked) serial_chars += sprintf(serial_buf + serial_chars, "%f\t", right_motor_out);

  // mark buffer as full
  if (print_locked) serial_buf_ready = true;

  // check if stop button is pressed
  //if (digitalRead(BUTTON_PIN) == 0) {
  //  //digitalWrite(13, HIGH);
  //  digitalWrite(MOTORL_1_PIN, 0);
  //  digitalWrite(MOTORL_2_PIN, 0);
  //  digitalWrite(MOTORR_1_PIN, 0);
  //  digitalWrite(MOTORR_2_PIN, 0);
  //  pid_timer.end();
  //  delay(200);
  //  setup();
  //}
}
