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
#define MAX_STRAIGHT_PWM 180
#define MAX_TURN_PWM 255

#define SERVO_KP 0.03

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

Servo servo;
float servo_position = 0;
float analog_cutoff = 200;

IntervalTimer pid_timer;
void setup()
{
  Serial1.begin(9600);

  servo.attach(SERVO_PIN);
  servo.write(90);

  for (int i = 0; i < NUM_SENSORS; i++) {
    pinMode(SENSOR_FIRST_PIN + i, INPUT);
  }

  // change ADC prescaler
  // sbi(ADCSRA, ADPS2);
  // cbi(ADCSRA, ADPS1);
  // cbi(ADCSRA, ADPS0);

  pinMode(MOTORL_1_PIN, OUTPUT);
  pinMode(MOTORL_2_PIN, OUTPUT);
  pinMode(MOTORL_PWM_PIN, OUTPUT);
  pinMode(MOTORR_1_PIN, OUTPUT);
  pinMode(MOTORR_2_PIN, OUTPUT);
  pinMode(MOTORR_PWM_PIN, OUTPUT);

  pinMode(13, OUTPUT);
  digitalWrite(13, 0);

  //Setup the pushbutton pin
  pinMode(BUTTON_PIN, INPUT);

  //Delay until the button is pressed
  delay(500);
  while(digitalRead(BUTTON_PIN) == 1);
  delay(500);

  pid_timer.begin(pid_interrupt, 1000);
}

uint16_t sensor_values = 0;

// 'l' for left, 'r' for right, 's' for straight
char servo_turn_direction = 's';

char serial_buf[600];
volatile bool serial_buf_ready = false;
int serial_chars = 0;

void loop() {

  if(digitalRead(BUTTON_PIN) == 0){
    digitalWrite(13, HIGH);
    digitalWrite(MOTORL_1_PIN, 0);
    digitalWrite(MOTORL_2_PIN, 0);
    digitalWrite(MOTORR_1_PIN, 0);
    digitalWrite(MOTORR_2_PIN, 0);
    delay(200);
    pid_timer.end();
    setup();
  }
  else{
    digitalWrite(13, LOW);
  }
  
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

  if (micros() - t > 1000 && t != 0) digitalWrite(13, 1);
  if (print_locked) serial_chars += sprintf(serial_buf + serial_chars, "%lu", micros() - t);
  t = micros();
  if (print_locked) serial_chars += sprintf(serial_buf + serial_chars, "\t");

  sensor_values = 0;
  for (int i = 0; i < NUM_SENSORS; i++) {
    sensor_values |= (analogRead(SENSOR_FIRST_PIN + i) > analog_cutoff) << i;
  }

  for (int i = 0; i < NUM_SENSORS; i++) {
    if (print_locked) serial_chars += sprintf(serial_buf + serial_chars, "%i", (sensor_values >> i) & 1);
  }
  if (print_locked) serial_chars += sprintf(serial_buf + serial_chars, "\t");

  int left_side_of_line = 0;
  while (sensor_values & _BV(7 - left_side_of_line) && left_side_of_line < NUM_SENSORS) {
    left_side_of_line++;
  }
  left_side_of_line--;

  int right_side_of_line = NUM_SENSORS - 1;
  while (sensor_values & _BV(7 - right_side_of_line) && right_side_of_line >= 0) {
    right_side_of_line--;
  }

  float center = (right_side_of_line + left_side_of_line - NUM_SENSORS + 1) / 2.0;

  if (print_locked) serial_chars += sprintf(serial_buf + serial_chars, "%i\t", left_side_of_line);
  if (print_locked) serial_chars += sprintf(serial_buf + serial_chars, "%i\t", right_side_of_line);
  if (print_locked) serial_chars += sprintf(serial_buf + serial_chars, "%f\t", center);

  if (left_side_of_line > right_side_of_line) {
    // entire view is white
    // turn in the same direction we were turning before
    switch (servo_turn_direction) {
      case 'l':
        servo_position = constrain(servo_position - SERVO_KP*NUM_SENSORS, -90, 90);
        break;
      case 'r':
      default:
        servo_position = constrain(servo_position + SERVO_KP*NUM_SENSORS, -90, 90);
        break;
    }
  } else if (left_side_of_line == -1 && right_side_of_line == NUM_SENSORS - 1) {
    // entire view is black
    if (servo_position < 0) {
      servo_position += SERVO_KP*NUM_SENSORS;
      servo_turn_direction = 'r';
    } else {
      servo_position -= SERVO_KP*NUM_SENSORS;
      servo_turn_direction = 'l';
    }
  } else {
    servo_position = constrain(servo_position + SERVO_KP*center, -90, 90);
    if (center > 0) {
      servo_turn_direction = 'r';
    } else if (center == 0) {
      servo_turn_direction = 's';
    } else {
      servo_turn_direction = 'l';
    }
  }

  if (print_locked) serial_chars += sprintf(serial_buf + serial_chars, "%f\t", servo_position);

  servo.write(servo_position + 90);

  digitalWrite(MOTORL_1_PIN, 0);
  digitalWrite(MOTORL_2_PIN, 1);
  digitalWrite(MOTORR_1_PIN, 0);
  digitalWrite(MOTORR_2_PIN, 1);

  analogWrite(MOTORL_PWM_PIN, constrain(MAX_STRAIGHT_PWM + MOTOR_KP*servo_position, 0, MAX_TURN_PWM));
  analogWrite(MOTORR_PWM_PIN, constrain(MAX_STRAIGHT_PWM - MOTOR_KP*servo_position, 0, MAX_TURN_PWM));

  if (print_locked) serial_chars += sprintf(serial_buf + serial_chars, "%f\t", constrain(MAX_STRAIGHT_PWM + MOTOR_KP*servo_position, 0, MAX_TURN_PWM));
  if (print_locked) serial_chars += sprintf(serial_buf + serial_chars, "%f\t", constrain(MAX_STRAIGHT_PWM - MOTOR_KP*servo_position, 0, MAX_TURN_PWM));

  if (MAX_STRAIGHT_PWM + MOTOR_KP*servo_position <= 0) {
    digitalWrite(MOTORL_2_PIN, 0);
  }
  if (MAX_STRAIGHT_PWM - MOTOR_KP*servo_position <= 0) {
    digitalWrite(MOTORR_2_PIN, 0);
  }

  if (print_locked) serial_buf_ready = true;
}
