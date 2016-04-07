#ifndef PROXSENSE_H
#define PROXSENSE_H

#define NUM_SENSORS 10

class ProxSense
{
private:
	int bar[NUM_SENSORS];
	int barPin[NUM_SENSORS];
	int weight[NUM_SENSORS];
	int setpoint;
	
public:
	ProxSense(int*, int*, int);
	void update();
	float generateAngle();
	bool sensorHigh();
	float readAngle();
 };
 
 #endif 
