#include "LightSensor.h"

#include <Arduino.h>

LightSensor::LightSensor(const uint8_t pin):
  mPin(pin)
{
  for (uint8_t i = 0; i < BUFFER_SIZE; i++)
    Update();
}

void LightSensor::Update()
{
  mRingBuffer.Add(analogRead(mPin));
}

uint8_t LightSensor::Brightness()
{
  uint8_t brightness = map(mRingBuffer.Mean(), 0, 1023, MIN, MAX);
  return constrain(brightness, MIN, MAX);
}
