
#include "app_level_interface.h"

void app_level_getDistanceCM(void) 
{
  digitalWrite(TRIG_PIN, LOW); // Set the trigger pin to low for 2uS
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH); // Send a 10uS high to trigger ranging
  delayMicroseconds(20);

  digitalWrite(TRIG_PIN, LOW); // Send pin low again
  double distance = pulseIn(ECHO_PIN, HIGH); // Read in times pulse

  CalculatedHight = (distance / 58) - BLIND_LENGTH; //Convert the pulse duration to distance
    
  Serial.println("Height");
  Serial.println(CalculatedHight, 3);

}

void app_level_Volume(void) 
{
  app_level_getDistanceCM();
  //You can add other math functions to calibrate it well
  CalculatedVolume = shapeTotalVolume * ((shapeHight-(CalculatedHight-BLIND_LENGTH))/shapeHight);

  Serial.println("shapeTotalVolume");
  Serial.println(shapeTotalVolume, 3);

  Serial.println("shapeHight");
  Serial.println(shapeHight, 3);
}