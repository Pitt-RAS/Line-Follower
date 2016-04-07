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
	movement_state = stop;

  //Setup the interrupt to call update
}

void Motion::update()
{
	//Update PID controllers
	//Compare current velocities to desired, using PID loop to calculate the new the settings to send to setVelRaw();
	
	//Below is probably not needed, I'll think about it in the morning
	/*switch(movement_state){
		case MovementType::charge:
			control_charge();
			break;
		case MovementType::search_arc:
			control_search_arc();
			break;
		case MovementType::deploy_ramps:
			control_deploy_ramps();
			break;
		case MovementType::stop:
			setVel(0, 0);
			break;
	}*/
}
//Private methods
void Motion::setVelRaw(Direction r, int pwmr, Direction l, int pwml){
	
	bool r_d;
	bool l_d;
	//Actually set things to the motor
	if(r == Direction::forward){
		r_d = true;
	} else{
		r_d = false;
	}
	
	//Actually set things to the motor
	if(l == Direction::forward){
		l_d = true;
	} else{
		l_d = false;
	}
	
	motor_rf.SetRaw(r_d, pwmr);
	motor_rb.SetRaw(r_d, pwmr);
	motor_lb.SetRaw(l_d, pwml);
	motor_lf.SetRaw(l_d, pwml);
	//motor_lf.Set(0.0, EnRead2() * 1000000 * MM_PER_STEP / 1000);

}

void Motion::setVel(float v, float w){
	//Do work to calculate individual motor velocities from angular acceleration and centerline velocity
	Direction directionl, directionr;

	
	
	float vl = v + ((PI * DISTANCE_BETWEEN_WHEELS * w)/2.0); //Replace with trig to calculate the desired wheel speed.
	float vr = v - ((PI * DISTANCE_BETWEEN_WHEELS * w)/2.0); //Replace with trig to calculate the deisred wheel speed.
	
	if(vl > 0){
		directionl = Direction::forward;
	}
	else{
		directionl = Direction::backward;
	}
	
	if(vr > 0){
		directionr = Direction::forward;
	}
	else{
		directionr = Direction::backward;
	}
	
	//Set the PID controllers to the v that is being targeted
	//For now just fudge it!
	
	int pwml = abs(vl) * (1024/5.0);
	int pwmr = abs(vr) * (1024/5.0);

	//For now just set the pwm values
	setVelRaw(directionr, pwmr, directionl, pwml);
}
