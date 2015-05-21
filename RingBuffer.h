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
    fixedpt sum = fixedpt_fromint(0);
    for (uint8_t i = 0; i < N; i++)
      sum += fixedpt_fromint(mBuffer[i]);
    sum = fixedpt_xdiv(sum, fixedpt_fromint(N));
    return static_cast<T>(fixedpt_toint(sum + fixedpt_rconst(0.5)));
  }

private:
  T mBuffer[N];
  uint8_t mPos;
};

#endif // RINGBUFFER_H
