#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

#include "RingBuffer.h"

class LightSensor
{
public:
  LightSensor(const uint8_t pin);

  /** @brief add new value to the ringbuffer */
  void Update();

  /** @brief obtain brightness in range 0-255 */
  uint8_t Brightness();

private:
  static const uint8_t BUFFER_SIZE = 16;

  RingBuffer<uint16_t, BUFFER_SIZE> mRingBuffer;
  uint8_t mPin;
};

#endif // LIGHTSENSOR_H
