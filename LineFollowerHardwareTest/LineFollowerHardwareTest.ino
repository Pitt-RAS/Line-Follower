#include <Servo.h> 
#include <EncoderMod.h>

//Uncomment or uncomment to test things, I recommend only doing one at a time
//#define TEST_MOTORS_ENCODERS
//#define TEST_SERVO
//#define TEST_IR_BAR

#define servo_delay 1

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
 
Servo myservo;  // create servo object to control a servo 
                // twelve servo objects can be created on most boards
 
int pos = 0;    // variable to store the servo position 

#define NUM_IR_SENSORS 12
int sensor_bar_pins[] = {IR0, IR1, IR2, IR3, IR4,
                         IR5, IR6, IR7, IR8, IR9,
                         IR10, IR11};

Encoder encoderRF(ENC_RF_1, ENC_RF_2);
Encoder encoderLF(ENC_LF_1, ENC_LF_2);
Encoder encoderRB(ENC_RB_1, ENC_RB_2); 
Encoder encoderLB(ENC_LB_1, ENC_LB_2);

void setup() 
{ 
  myservo.attach(25);  // attaches the servo on pin 9 to the servo object
  

  pinMode(MOTOR_RF_1, OUTPUT);
  pinMode(MOTOR_RF_2, OUTPUT);
  
  pinMode(MOTOR_RB_1, OUTPUT);
  pinMode(MOTOR_RB_2, OUTPUT);
  
  pinMode(MOTOR_LF_1, OUTPUT);
  pinMode(MOTOR_LF_2, OUTPUT);
  
  pinMode(MOTOR_LB_1, OUTPUT);
  pinMode(MOTOR_LB_2, OUTPUT);


  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  Serial.begin(9600);
} 
 
void loop() 
{ 

#ifdef TEST_IR_BAR
  for(int i = 2; i < NUM_IR_SENSORS; i++){
//    Serial.print(i);
//    Serial.print(":");
//    Serial.print(analogRead(sensor_bar_pins[i]));
//    Serial.print(" ");

    if(analogRead(sensor_bar_pins[i]) > 500){
      Serial.print("0");
    }
    else{
      Serial.print("1");
    }
  }
  Serial.println();
  delay(100);
#endif

#ifdef TEST_SERVO
  //Test servo
  for(int end = 0; end < 150; end+=5 ){
    for(pos = 0; pos <= end; pos += 1) // goes from 0 degrees to 180 degrees 
    {                                  // in steps of 1 degree 
      myservo.write(pos);              // tell servo to go to position in variable 'pos' 
      delay(servo_delay);                       // waits 15ms for the servo to reach the position 
    } 
    for(pos = end; pos>=0; pos-=1)     // goes from 180 degrees to 0 degrees 
    {                                
      myservo.write(pos);              // tell servo to go to position in variable 'pos' 
      delay(servo_delay);                       // waits 15ms for the servo to reach the position 
    } 
  }
#endif

#ifdef TEST_MOTORS_ENCODERS
  //Test Motor RF
  analogWrite(MOTOR_RF_PWM, 255);
  digitalWrite(MOTOR_RF_1, HIGH);
  digitalWrite(MOTOR_RF_2, LOW);
  delay(1000);
  Serial.print("RF:");
  Serial.println(encoderRF.stepRate()*10000.0);
  delay(1000);
  digitalWrite(MOTOR_RF_1, LOW);
  digitalWrite(MOTOR_RF_2, HIGH);
  delay(1000);
  Serial.print("RF:");
  Serial.println(encoderRF.stepRate()*10000.0);
  delay(1000);
  analogWrite(MOTOR_RF_PWM, 0);

  //Test Motor LF
  analogWrite(MOTOR_LF_PWM, 255);
  digitalWrite(MOTOR_LF_1, HIGH);
  digitalWrite(MOTOR_LF_2, LOW);
  delay(1000);
  Serial.print("LF:");
  Serial.println(encoderLF.stepRate()*10000.0);
  delay(1000);
  digitalWrite(MOTOR_LF_1, LOW);
  digitalWrite(MOTOR_LF_2, HIGH);
  delay(1000);
  Serial.print("LF:");
  Serial.println(encoderLF.stepRate()*10000.0);
  delay(1000);
  analogWrite(MOTOR_LF_PWM, 0);

//  //Test Motor RB
  analogWrite(MOTOR_RB_PWM, 255);
  digitalWrite(MOTOR_RB_1, HIGH);
  digitalWrite(MOTOR_RB_2, LOW);
  delay(1000);
  Serial.print("RB:");
  Serial.println(encoderRB.stepRate()*10000.0);
  delay(1000);
  digitalWrite(MOTOR_RB_1, LOW);
  digitalWrite(MOTOR_RB_2, HIGH);
  delay(1000);
  Serial.print("RB:");
  Serial.println(encoderRB.stepRate()*10000.0);
  delay(1000);  analogWrite(MOTOR_RB_PWM, 0);

  //Test Motor LB
  analogWrite(MOTOR_LB_PWM, 255);
  digitalWrite(MOTOR_LB_1, HIGH);
  digitalWrite(MOTOR_LB_2, LOW);
  delay(1000);
  Serial.print("LB:");
  Serial.println(encoderLB.stepRate()*10000.0);
  delay(1000);
  digitalWrite(MOTOR_LB_1, LOW);
  digitalWrite(MOTOR_LB_2, HIGH);
  delay(1000);
  Serial.print("LB:");
  Serial.println(encoderLB.stepRate()*10000.0);
  delay(1000);
  analogWrite(MOTOR_LB_PWM, 0);
#endif

  
} 
