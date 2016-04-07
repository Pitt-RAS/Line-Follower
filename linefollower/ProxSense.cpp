#include "ProxSense.h"
#include "Arduino.h"
#include "conf.h"

ProxSense::ProxSense(int barPin[NUM_SENSORS], int weight[NUM_SENSORS], int setpoint)
{
  memcpy(this->barPin, barPin, NUM_SENSORS*sizeof(int));
  memcpy(this->weight, weight, NUM_SENSORS*sizeof(int));
  this->setpoint = setpoint;
}

void ProxSense::update()
{
	for(int ii = 0; ii < NUM_SENSORS; ii++)
	{
		bar[ii] = analogRead(barPin[ii]);
		Serial.print(ii);
		Serial.print(": ");
		Serial.print(bar[ii]);
		Serial.print(" ");
	}
	Serial.println();
}

float ProxSense::generateAngle()
{
	int sum = 0;
	float angle;
	int numActive = 0;
	for(int ii = 0; ii < NUM_SENSORS; ii++)
	{
		if(bar[ii] < setpoint){
			numActive++;
			sum+=weight[ii];
		}
	}
	
	if(numActive > 0){
		angle = (float)sum/numActive;
	}
	else {
		angle = PROXIMITY_INACTIVE; //Set to some error that we will never reach
	}
	Serial.println(angle);

	return angle;
}

float ProxSense::readAngle() {
	update();
	return generateAngle();
}
