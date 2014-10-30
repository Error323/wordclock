#ifndef ANIMATION_H
#define ANIMATION_H

#include <stdint.h>
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

template<uint8_t SIZE>
class Animation {
public:
  Animation(Adafruit_NeoPixel &led_matrix);
  virtual ~Animation() {}

  virtual void Reset();
  virtual void Tick() = 0;
  virtual bool IsDone() = 0;

protected:
  void SetPixel(uint8_t i, uint8_t j, const uint32_t p);

  struct Slider {
    uint8_t speed; // speed in fps
    uint8_t start; // start time in frames
    uint8_t frame; // current frame
    uint8_t asize; // animation length
  }

  static uint8_t sFrame;
  static uint8_t sDone;
  static Slider sSliders[SIZE];

private:
  Adafruit_NeoPixel &sLedMatrix;
}

#endif // ANIMATION_H
/*
float normal(const float x, float sigma_squared)
{
  return expf(-(x*x) / (2.0f * sigma_squared));
}

uint8_t idx(const uint8_t i, uint8_t j)
{
  if (i % 2 == 1)
    j = SIZE - 1 - j;
  return i*SIZE + j;
}

void ani_matrix(const uint32_t activated)
{
  static uint8_t speed[SIZE]; // speed in fps
  static uint8_t start[SIZE]; // start time in frames
  static uint8_t frame[SIZE]; // current frame
  static uint8_t asize[SIZE]; // animation length
  static uint8_t done = 0;
  static uint8_t frame_time = 0;
  static uint32_t prev_activated = 0ul;
  static uint32_t tmp = 0ul;
  uint8_t i, j, c;

  if (activated != tmp)
  {
    for (i = 0; i < SIZE; i++)
    {
      speed[i] = random(1, 6);
      start[i] = random(0, 10);
      asize[i] = random(5, SIZE);
      frame[i] = 0;
    }
    done = 0;
    frame_time = 0;
    prev_activated = tmp;
    tmp = activated;
  }

  else if (done < SIZE)
  {
    for (i = 0; i < SIZE; i++)
    {
      for (j = 0; j < SIZE; j++)
      {
        // perform animation tick
        if (frame[j] >= start[j] + i && frame[j] < asize[j] + start[j] + i)
        {
          if (frame[j] > start[j] + i && wc::matrix[i*SIZE+j] & activated)
            led_matrix.setPixelColor(idx(i,j), color);
          else
          {
            c = round(normal(frame[j]-i-start[j], asize[j]-3) * 255);
            led_matrix.setPixelColor(idx(i,j), c, c, c);
          }
        }
        // leave (prev)activated as is, turn rest off
        else if ((wc::matrix[i*SIZE+j] & (prev_activated|activated) & 0x7fffff) == 0ul)
          led_matrix.setPixelColor(idx(i,j), 0);
      }
    }

    // increase counters, check if done
    done = 0;
    for (i = 0; i < SIZE; i++)
    {
      if (frame[i] > asize[i] + start[i] + SIZE)
        done++;
      if (frame_time % speed[i] == 0)
        frame[i]++;
    }
    frame_time++;
  }
}
*/
