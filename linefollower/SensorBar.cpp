#include "SensorBar.h"
#include "Arduino.h"
#include "conf.h"

SensorBar::SensorBar(int bar_pin[NUM_SENSORS], int weight[NUM_SENSORS], int setpoint)
{
  memcpy(this->bar_pin_, bar_pin, NUM_SENSORS * sizeof(int));
  memcpy(this->weight_, weight, NUM_SENSORS * sizeof(int));
  this->setpoint_ = setpoint;
}

void SensorBar::update()
{
  for (int i = 0; i < NUM_SENSORS; i++) {
    bar_[i] = analogRead(bar_pin_[i]);
    Serial.print(i);
    Serial.print(": ");
    Serial.print(bar_[i]);
    Serial.print(" ");
  }
  Serial.println();
}

float SensorBar::generateAngle()
{
  int sum = 0;
  float angle;
  int num_active = 0;
  for (int i = 0; i < NUM_SENSORS; i++) {
    if (bar_[i] < setpoint) {
      num_active++;
      sum += weight_[i];
    }
  }
  
  if (num_active > 0) {
    angle = (float)sum / num_active;
  } else {
    angle = PROXIMITY_INACTIVE; //Set to some error that we will never reach
  }
  Serial.println(angle);

  return angle;
}

float SensorBar::readAngle()
{
  update();
  return generateAngle();
}
