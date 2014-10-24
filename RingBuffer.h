#include <stdint.h>

template<typename T, uint8_t N=10>
class RingBuffer
{
public:
  RingBuffer(): mPos(0), mSize(0) {}

  ~RingBuffer() {}

  void Add(const T value)
  {
    mBuffer[mPos] = value;
    mSize = mSize < N ? mSize + 1 : N;
    mPos++;
    mPos %= N;
  }

  T Avg()
  {
    float sum = 0.0f;
    for (uint8_t i = 0; i < mSize; i++)
      sum += mBuffer[i];
    return static_cast<T>(sum / mSize + 0.5f);
  }

  uint8_t BufferSize()
  {
    return mSize;
  }

private:
  T mBuffer[N];
  uint8_t mPos;
  uint8_t mSize;
};
