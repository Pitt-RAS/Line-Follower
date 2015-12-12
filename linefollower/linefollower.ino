#include <Servo.h>

#define SENSOR_FIRST_PIN 2
#define NUM_SENSORS 8
#define MOTORA_PWM_PIN 11
#define MOTORB_PWM_PIN 10
#define MOTORA_1_PIN A3
#define MOTORA_2_PIN A2
#define MOTORB_1_PIN A1
#define MOTORB_2_PIN A0

#define MOTOR_KP 5
#define MAX_STRAIGHT_PWM 180
#define MAX_TURN_PWM 180

#define SERVO_KP 0.03

Servo servo;
float servo_position = 0;

void setup()
{
  Serial.begin(9600);

  servo.attach(12);
  servo.write(90);

  for (int i = 0; i < NUM_SENSORS; i++) {
    pinMode(SENSOR_FIRST_PIN + i, INPUT);
  }
  pinMode(MOTORA_1_PIN, OUTPUT);
  pinMode(MOTORA_2_PIN, OUTPUT);
  pinMode(MOTORA_PWM_PIN, OUTPUT);
  pinMode(MOTORB_1_PIN, OUTPUT);
  pinMode(MOTORB_2_PIN, OUTPUT);
  pinMode(MOTORB_PWM_PIN, OUTPUT);
}

uint8_t sensor_values = 0;

// 'l' for left, 'r' for right, 's' for straight
// char turn_direction = 's';

void loop()
{
  sensor_values = 0;
  for (int i = 0; i < NUM_SENSORS; i++) {
    if (digitalRead(SENSOR_FIRST_PIN + NUM_SENSORS - 1 - i)) {
      sensor_values |= _BV(i);
      // Serial.print(1);
    } else {
      // Serial.print(0);
    }
  }
  // Serial.println();

  int left_side_of_line = 0;
  while (sensor_values & _BV(left_side_of_line) && left_side_of_line < NUM_SENSORS) {
    left_side_of_line++;
  }
  left_side_of_line--;

  int right_side_of_line = NUM_SENSORS - 1;
  while (sensor_values & _BV(right_side_of_line) && right_side_of_line >= 0) {
    right_side_of_line--;
  }

  float center = (right_side_of_line + left_side_of_line - NUM_SENSORS + 1) / 2.0;

  if (left_side_of_line > right_side_of_line) {
    if (servo_position > 0) {
      servo_position = 90;
    } else {
      servo_position = -90;
    }
    
    // // turn in the same direction we were turning before
    // switch (turn_direction) {
    //   case 'l':
    //     digitalWrite(MOTORA_1_PIN, 0);
    //     digitalWrite(MOTORA_2_PIN, 0);
    //     analogWrite(MOTORA_PWM_PIN, 0);
    //     digitalWrite(MOTORB_1_PIN, 0);
    //     digitalWrite(MOTORB_2_PIN, 1);
    //     analogWrite(MOTORB_PWM_PIN, MAX_TURN_PWM);
    //     break;
    //   case 'r':
    //   default:
    //     digitalWrite(MOTORA_1_PIN, 0);
    //     digitalWrite(MOTORA_2_PIN, 1);
    //     analogWrite(MOTORA_PWM_PIN, MAX_TURN_PWM);
    //     digitalWrite(MOTORB_1_PIN, 0);
    //     digitalWrite(MOTORB_2_PIN, 0);
    //     analogWrite(MOTORB_PWM_PIN, 0);
    //     break;
    // }

    // return;
  } else {
    servo_position = constrain(servo_position + SERVO_KP*center, -90, 90);
  }

  servo.write(servo_position + 90);

  digitalWrite(MOTORA_1_PIN, 0);
  digitalWrite(MOTORA_2_PIN, 1);
  digitalWrite(MOTORB_1_PIN, 0);
  digitalWrite(MOTORB_2_PIN, 1);

  // if (servo_position < 0) {
  //   turn_direction = 'l';
  // } else if (servo_position == 0) {
  //   turn_direction = 's';
  // } else {
  //   turn_direction = 'r';
  // }

  analogWrite(MOTORA_PWM_PIN, constrain(MAX_STRAIGHT_PWM + MOTOR_KP*servo_position, 0, MAX_TURN_PWM));
  analogWrite(MOTORB_PWM_PIN, constrain(MAX_STRAIGHT_PWM - MOTOR_KP*servo_position, 0, MAX_TURN_PWM));

  if (MAX_STRAIGHT_PWM + MOTOR_KP*servo_position <= 0) {
    digitalWrite(MOTORA_2_PIN, 0);
  }
  if (MAX_STRAIGHT_PWM - MOTOR_KP*servo_position <= 0) {
    digitalWrite(MOTORB_2_PIN, 0);
  }
}
