#ifndef SENSORBAR_H_
#define SENSORBAR_H_

#include "conf.h"

class SensorBar {
 private:
  int bar_[NUM_SENSORS];
  int bar_pin_[NUM_SENSORS];
  int weight_[NUM_SENSORS];
  int setpoint_;
  
 public:
  SensorBar(int bar_pin[NUM_SENSORS], int weight[NUM_SENSORS], int setpoint);

  void update();
  float generateAngle();
  bool sensorHigh();
  float readAngle();
};
 
 #endif 
