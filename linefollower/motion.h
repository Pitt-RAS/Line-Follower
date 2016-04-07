#ifndef MICROMOUSE_MOTION_H_
#define MICROMOUSE_MOTION_H_

#include "Arduino.h"
#include "conf.h"
#include "encoding.h"
#include "motors.h"
#include "PIDController.h"

enum Direction { forward, backward };
enum MovementType {charge, search_arc, deploy_ramps, stop};

class Motion
{
private:
	Motor motor_lf;
	Motor motor_rf;
	Motor motor_lb;
	Motor motor_rb;
	PIDController pid_lf;
	PIDController pid_lb;
	PIDController pid_rf;
	PIDController pid_rb;

	
	MovementType movement_state;
	
	//Private motion functions
	void setVelRaw(Direction r, int pwmr, Direction l, int pwml);

public:
  Motion();
	void setVel(float v, float w);

	
	//Must be called a set rate
	void update();

 };
 
#endif
