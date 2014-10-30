#include "Animation.h"

Animation::Animation(Adafruit_NeoPixel &led_matrix):
  sLedMatrix(led_matrix)
{
  Reset();
}

void Animation::SetPixel(uint8_t i, uint8_t j, const uint32_t p)
{
  if (i % 2 == 1)
    j = SIZE - 1 - j;
  sLedMatrix.setPixelColor(i*SIZE+j, p);
}

void Animation::Reset()
{
  for (uint8_t i = 0; i < SIZE; i++)
  {
    sSlider[i].speed = random(1, 6);
    sSlider[i].start = random(0, 10);
    sSlider[i].asize = random(5, SIZE);
    sSlider[i].frame = 0;
  }
  sFrame = sDone = 0;
}
