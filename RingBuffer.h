#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdint.h>
#include "fixedptc.h"

template<typename T, uint8_t N>
class RingBuffer
{
public:
  RingBuffer(): mPos(0) {}
  ~RingBuffer() {}

  void Add(const T value)
  {
    mBuffer[mPos] = value;
    mPos++;
    mPos %= N;
  }

  T Mean()
  {
    uint32_t sum = 0ul;
    for (uint8_t i = 0; i < N; i++)
      sum += mBuffer[i];
    sum /= N;
    return static_cast<T>(sum);
  }

private:
  T mBuffer[N];
  uint8_t mPos;
};

#endif // RINGBUFFER_H
