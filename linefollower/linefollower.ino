#include <Servo.h>

#define SENSOR_FIRST_PIN A0
#define NUM_SENSORS 8
#define MOTORA_PWM_PIN 6
#define MOTORB_PWM_PIN 5
#define MOTORA_1_PIN 7
#define MOTORA_2_PIN 8
#define MOTORB_1_PIN 10
#define MOTORB_2_PIN 9

#define MOTOR_KP 10
#define MAX_STRAIGHT_PWM 180
#define MAX_TURN_PWM 255

#define SERVO_KP 0.2
#define SENSOR_KP 0.2

#define SERVO_OFFSET 20 //Our software thinks that 0 degrees is center but this is off, this is a fudge factor

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

Servo servo;
float servo_position = 0;

float atan_lookup[17];

void setup()
{
   Serial.begin(9600);

  servo.attach(12);
  servo.write(90);

  for (int i = 0; i < NUM_SENSORS; i++) {
    pinMode(SENSOR_FIRST_PIN + i, INPUT);
  }

  //Setup the arc tangent lookup table, calculating arctangent on the fly would be very slow.
  //This makes the worse case linear since we only need certain values anyway
  for (int i = 0; i < NUM_SENSORS*2 + 1; i++){
    //The sensors are 5.625mm apart and 50mm from the rotation point, convert to degrees since that is what we use for the rest
    float pos = (i-NUM_SENSORS)/2.0;
    atan_lookup[i] = atan(pos*5.625/50) * 180.0 / PI;
    Serial.println(atan_lookup[i]);
  }

  // change ADC prescaler
  sbi(ADCSRA,ADPS2);
  cbi(ADCSRA,ADPS1);
  cbi(ADCSRA,ADPS0);

  pinMode(MOTORA_1_PIN, OUTPUT);
  pinMode(MOTORA_2_PIN, OUTPUT);
  pinMode(MOTORA_PWM_PIN, OUTPUT);
  pinMode(MOTORB_1_PIN, OUTPUT);
  pinMode(MOTORB_2_PIN, OUTPUT);
  pinMode(MOTORB_PWM_PIN, OUTPUT);
}

uint8_t sensor_values = 0;

// 'l' for left, 'r' for right, 's' for straight
char servo_turn_direction = 's';

void loop()
{
  sensor_values = PINC;
  sensor_values |= (analogRead(A6) > 1) << 6;
  // delay(10);
  sensor_values |= (analogRead(A7) > 1) << 7;

  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print((sensor_values >> i) & 1);
  }
  Serial.print("\t");

  int left_side_of_line = 0;
  while (sensor_values & _BV(7 - left_side_of_line) && left_side_of_line < NUM_SENSORS) {
    left_side_of_line++;
  }
  left_side_of_line--; //Decrement by one so 0 = leftmost

  int right_side_of_line = NUM_SENSORS-1;
  while (sensor_values & _BV(7 - right_side_of_line) && right_side_of_line >= 0) {
    right_side_of_line--;
  }
  right_side_of_line++;

  int line_width = right_side_of_line - left_side_of_line;
  float center = ((right_side_of_line + left_side_of_line) / 2.0) - 3.5;

  float angle_sensors = atan_lookup[(int)(center*2) + NUM_SENSORS];

  Serial.print(left_side_of_line);
  Serial.print("\t");
  Serial.print(right_side_of_line);
  Serial.print("\t");
  Serial.print(center);
  Serial.print("\t");
  Serial.print(angle_sensors);
  Serial.print("\t");

//  if (left_side_of_line > right_side_of_line) {
//    // entire view is white
//    // turn in the same direction we were turning before
//    switch (servo_turn_direction) {
//      case 'l':
//        servo_position = constrain(servo_position - SERVO_KP*NUM_SENSORS, -90, 90);
//        break;
//      case 'r':
//      default:
//        servo_position = constrain(servo_position + SERVO_KP*NUM_SENSORS, -90, 90);
//        break;
//    }
//  } else if (left_side_of_line == -1 && right_side_of_line == NUM_SENSORS - 1) {
//    // entire view is black
//    if (servo_position < 0) {
//      servo_position += SERVO_KP*NUM_SENSORS;
//      servo_turn_direction = 'r';
//    } else {
//      servo_position -= SERVO_KP*NUM_SENSORS;
//      servo_turn_direction = 'l';
//    }
//  } else {
//    servo_position = constrain(servo_position + SERVO_KP*center, -90, 90);
//    if (center > 0) {
//      servo_turn_direction = 'r';
//    } else if (center == 0) {
//      servo_turn_direction = 's';
//    } else {
//      servo_turn_direction = 'l';
//    }
//  }

  Serial.print(servo_position);
  Serial.print("\t");

  servo.write(servo_position + 90 + SERVO_OFFSET);

  digitalWrite(MOTORA_1_PIN, 0);
  digitalWrite(MOTORA_2_PIN, 1);
  digitalWrite(MOTORB_1_PIN, 0);
  digitalWrite(MOTORB_2_PIN, 1);

  //analogWrite(MOTORA_PWM_PIN, constrain(MAX_STRAIGHT_PWM + MOTOR_KP*servo_position + SENSOR_KP*angle_sensors, 0, MAX_TURN_PWM));
  //analogWrite(MOTORB_PWM_PIN, constrain(MAX_STRAIGHT_PWM - MOTOR_KP*servo_position + SENSOR_KP*angle_sensors, 0, MAX_TURN_PWM));

  Serial.print(constrain(MAX_STRAIGHT_PWM + MOTOR_KP*servo_position + SENSOR_KP*angle_sensors, 0, MAX_TURN_PWM));
  Serial.print("\t");
  Serial.print(constrain(MAX_STRAIGHT_PWM - MOTOR_KP*servo_position + SENSOR_KP*angle_sensors, 0, MAX_TURN_PWM));
  Serial.print("\t");

  if (MAX_STRAIGHT_PWM + MOTOR_KP*servo_position <= 0) {
    digitalWrite(MOTORA_2_PIN, 0);
  }
  if (MAX_STRAIGHT_PWM - MOTOR_KP*servo_position <= 0) {
    digitalWrite(MOTORB_2_PIN, 0);
  }

  Serial.println();
}
