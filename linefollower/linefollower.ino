#include <Servo.h>

#define SENSOR_FIRST_PIN A0
#define NUM_SENSORS 8
#define MOTORL_PWM_PIN 6
#define MOTORR_PWM_PIN 9
#define MOTORL_1_PIN 8
#define MOTORL_2_PIN 10
#define MOTORR_1_PIN 12
#define MOTORR_2_PIN 11
#define BUTTON_PIN 2

#define SERVO_PIN 5

#define MOTOR_KP 2
#define MAX_STRAIGHT_PWM 80
#define MAX_TURN_PWM 135

#define SERVO_KP 0.04
#define SERVO_HOLD_WIDTH 1
#define SERVO_KI 0
#define SERVO_KD 0.0009

#define SERVO_INTEGRAL_CAP 0

// time between interrupt calls (in us)
#define DT 1000

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

Servo servo;
float servo_position = 0;
float servo_integral = 0;
int last_center = 0;

float analog_cutoff = 150;

uint16_t sensor_values = 0;

// 'l' for left, 'r' for right, 's' for straight
char servo_turn_direction = 's';

char serial_buf[600];
volatile bool serial_buf_ready = false;
int serial_chars = 0;

IntervalTimer pid_timer;
void setup()
{
  Serial1.begin(9600);

  // set up servo
  servo.attach(SERVO_PIN);
  servo.write(90);

  for (int i = 0; i < NUM_SENSORS; i++) {
    pinMode(SENSOR_FIRST_PIN + i, INPUT);
  }

  pinMode(MOTORL_1_PIN, OUTPUT);
  pinMode(MOTORL_2_PIN, OUTPUT);
  pinMode(MOTORL_PWM_PIN, OUTPUT);
  pinMode(MOTORR_1_PIN, OUTPUT);
  pinMode(MOTORR_2_PIN, OUTPUT);
  pinMode(MOTORR_PWM_PIN, OUTPUT);

  // set up onboard led
  pinMode(13, OUTPUT);
  digitalWrite(13, 0);

  //Setup the pushbutton pin
  pinMode(BUTTON_PIN, INPUT);

  //Delay until the button is pressed
  delay(500);
  while(digitalRead(BUTTON_PIN) == 1);
  delay(500);

  // log initial sensor values
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial1.print(analogRead(SENSOR_FIRST_PIN + i));
    Serial1.print("\t");
  }
  Serial1.println();

  // start timed pid loop interrupt
  pid_timer.begin(pid_interrupt, DT);
}

void loop() {
  while (!serial_buf_ready) {}

  int serial_chars_printed = 0;

  while (serial_chars_printed < serial_chars) {
    for (int i = 0; serial_chars_printed < serial_chars && i < 300; i++) {
      Serial1.write(serial_buf[serial_chars_printed]);
      serial_chars_printed++;
    }
    delay(30);
  }

  Serial1.write('\r');
  Serial1.write('\n');
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
  for (int i = 0; i < NUM_SENSORS; i++) {
    sensor_values |= (analogRead(SENSOR_FIRST_PIN + i) < analog_cutoff) << i;
  }

  // log photosensor data
  for (int i = 0; i < NUM_SENSORS; i++) {
    if (print_locked) serial_chars += sprintf(serial_buf + serial_chars, "%i", (sensor_values >> i) & 1);
  }
  if (print_locked) serial_chars += sprintf(serial_buf + serial_chars, "\t");

  // find center of line in photosensor data
  int left_side_of_line = 0;
  while (sensor_values & _BV(left_side_of_line) && left_side_of_line < NUM_SENSORS) {
    left_side_of_line++;
  }

  int right_side_of_line = NUM_SENSORS - 1;
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
        center = -NUM_SENSORS;
        break;
      case 'r':
      default:
        center = NUM_SENSORS;
        break;
    }
  } else if (left_side_of_line == 0 && right_side_of_line == NUM_SENSORS - 1) {
    // entire view is black, send sensor back to center
    if (servo_position < 0) {
      center = NUM_SENSORS;
    } else {
      center = -NUM_SENSORS;
    }
  } else {
    // line is in view
    center = left_side_of_line + right_side_of_line - NUM_SENSORS + 1; 
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
  if (left_motor_out > 0) {
    digitalWrite(MOTORL_1_PIN, 0);
    digitalWrite(MOTORL_2_PIN, 1);
  } else {
    digitalWrite(MOTORL_1_PIN, 1);
    digitalWrite(MOTORL_2_PIN, 0);
  }

  if (right_motor_out > 0) {
    digitalWrite(MOTORR_1_PIN, 0);
    digitalWrite(MOTORR_2_PIN, 1);
  } else {
    digitalWrite(MOTORR_1_PIN, 1);
    digitalWrite(MOTORR_2_PIN, 0);
  }

  analogWrite(MOTORL_PWM_PIN, abs(left_motor_out));
  analogWrite(MOTORR_PWM_PIN, abs(right_motor_out));

  // log motor speeds
  if (print_locked) serial_chars += sprintf(serial_buf + serial_chars, "%f\t", left_motor_out);
  if (print_locked) serial_chars += sprintf(serial_buf + serial_chars, "%f\t", right_motor_out);

  // mark buffer as full
  if (print_locked) serial_buf_ready = true;

  // check if stop button is pressed
  if (digitalRead(BUTTON_PIN) == 0) {
    //digitalWrite(13, HIGH);
    digitalWrite(MOTORL_1_PIN, 0);
    digitalWrite(MOTORL_2_PIN, 0);
    digitalWrite(MOTORR_1_PIN, 0);
    digitalWrite(MOTORR_2_PIN, 0);
    pid_timer.end();
    delay(200);
    setup();
  }
}
