#include "motion.h"

Motion::Motion()
	:motor_lf (MOTOR_LF_1, MOTOR_LF_2, MOTOR_LF_PWM),
	motor_rf (MOTOR_RF_1, MOTOR_RF_2, MOTOR_RF_PWM),
	motor_lb (MOTOR_LB_1, MOTOR_LB_2, MOTOR_LB_PWM),
	motor_rb (MOTOR_RB_1, MOTOR_RB_2, MOTOR_RB_PWM),
	pid_lf (KP_VELOCITY, KI_VELOCITY, KD_VELOCITY),
	pid_lb (KP_VELOCITY, KI_VELOCITY, KD_VELOCITY),
	pid_rf (KP_VELOCITY, KI_VELOCITY, KD_VELOCITY),
	pid_rb (KP_VELOCITY, KI_VELOCITY, KD_VELOCITY)
{
  //Setup the interrupt to call update
}

void Motion::update()
{
	//Update PID controllers
	//Compare current velocities to desired, using PID loop to calculate the new the settings to send to setVelRaw();
}
//Private methods
void Motion::setVelRaw(bool r, int pwmr, bool l, int pwml){
	
	motor_rf.SetRaw(r, pwmr);
	motor_rb.SetRaw(r, pwmr);
	motor_lb.SetRaw(l, pwml);
	motor_lf.SetRaw(l, pwml);
}

void Motion::setVel(float v, float w){
	//Do work to calculate individual motor velocities from angular acceleration and centerline velocity
	bool directionl, directionr;

	float vl = v + ((PI * DISTANCE_BETWEEN_WHEELS * w)/2.0); //Replace with trig to calculate the desired wheel speed.
	float vr = v - ((PI * DISTANCE_BETWEEN_WHEELS * w)/2.0); //Replace with trig to calculate the deisred wheel speed.
	
	if(vl > 0){
		directionl = true;
	}
	else{
		directionl = false;
	}
	
	if(vr > 0){
		directionr = true;
	}
	else{
		directionr = false;
	}
	
	//Set the PID controllers to the v that is being targeted
	//For now just fudge it!
	
	int pwml = abs(vl) * (1024/5.0);
	int pwmr = abs(vr) * (1024/5.0);

	//For now just set the pwm values
	setVelRaw(directionr, pwmr, directionl, pwml);
}
