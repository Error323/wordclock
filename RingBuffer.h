#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdint.h>

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
    float sum = 0.0f;
    for (uint8_t i = 0; i < N; i++)
      sum += mBuffer[i];
    sum /= N;
    return static_cast<T>(sum + 0.5f);
  }

private:
  T mBuffer[N];
  uint8_t mPos;
};

#endif // RINGBUFFER_H
