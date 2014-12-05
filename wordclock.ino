/**
 * Wordclock with various easter eggs
 *
 * @author F. Huizinga 
 * @date Sun Oct 17:25:37 CEST 2014
 */

#include <Wire.h>
#include <SPI.h>
#include <RTClib.h>
#include <RTC_DS3231.h>
#include <Adafruit_NeoPixel.h>
#include <Time.h>
#include <Timezone.h>

#include "WordClock.h"
#include "LightSensor.h"

/* Settings */
#define LIGHT_PIN 0 // Light sensor pin
#define LED_PIN   7 // Matrix led pin
#define SIZE     10 // Matrix board size x size
#define FPS      40 // Frames per second to achieve

/* Globals */
/** @brief color */
struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

/** @brief earthtone colors
 * http://www.creativecolorschemes.com/resources/free-color-schemes/earth-tone-color-scheme.shtml
 */
static Color colors[] = { { 73, 58, 41 },
                          { 120, 109, 91 },
                          { 169, 161, 140 },
                          { 97, 51, 24 },
                          { 133, 87, 35 },
                          { 185, 156, 107 },
                          { 143, 59, 27 },
                          { 213, 117, 0 },
                          { 219, 202, 105 },
                          { 64, 79, 36 },
                          { 102, 141, 60 },
                          { 189, 208, 156 },
                          { 78, 97, 114 },
                          { 131, 146, 159 },
                          { 163, 172, 184 } };

static Color color = colors[random(0,15)];

/** @brief the hardware led matrix */
static Adafruit_NeoPixel led_matrix(SIZE*SIZE, LED_PIN);

/** @brief the realtime clock */
static RTC_DS3231 rtc;

static TimeChangeRule summer_time = {"CEST", Last, Sat, Mar, 2, 120};
static TimeChangeRule winter_time = {"CET", Last, Sat, Oct, 2, 60};
static Timezone time_zone(summer_time, winter_time);
time_t time;

/** @brief the light sensor */
static LightSensor light_sensor(LIGHT_PIN);

/** @brief animation variables */
struct Line {
  uint8_t speed;
  uint8_t start;
  uint8_t frame;
  uint8_t asize;
};

static Line L[SIZE];
static uint8_t done = 0;
static uint8_t frame_time = 0;

/** @brief compute normal distribution given s^2 and mu=0 */
float normal(const float x, float sigma_squared)
{
  return expf(-(x*x) / (2.0f * sigma_squared));
}

/** @brief invert uneven rows as our matrix is soldered as a strip */
uint8_t idx(const uint8_t i, uint8_t j)
{
  if (i % 2 == 1)
    j = SIZE - 1 - j;
  return i*SIZE + j;
}

void reset()
{
  for (uint8_t i = 0; i < SIZE; i++)
  {
    L[i].speed = random(1, 6);
    L[i].start = random(0, 10);
    L[i].asize = random(5, SIZE);
    L[i].frame = 0;
  }
  done = 0;
  frame_time = 0;
  color = colors[random(0, 15)];
}

void ani_matrix(const uint32_t activated, const uint32_t previous)
{
  uint8_t i, j, c;

  if (done < SIZE)
  {
    for (i = 0; i < SIZE; i++)
    {
      for (j = 0; j < SIZE; j++)
      {
        // perform animation tick
        if (L[j].frame >= L[j].start + i && L[j].frame < L[j].asize + L[j].start + i)
        {
          if (L[j].frame > L[j].start + i && wc::matrix[i*SIZE+j] & activated)
            c = 200;
          else
            c = round(normal(L[j].frame-i-L[j].start, L[j].asize-3) * 255);

          led_matrix.setPixelColor(idx(i,j), c, c, c);
        }
        // leave (prev)activated as is, turn rest off
        else if ((wc::matrix[i*SIZE+j] & (previous|activated) & 0x7fffff) == 0ul)
          led_matrix.setPixelColor(idx(i,j), 0);
      }
    }

    // increase counters, check if done
    done = 0;
    for (i = 0; i < SIZE; i++)
    {
      if (L[i].frame > L[i].asize + L[i].start + SIZE)
        done++;
      if (frame_time % L[i].speed == 0)
        L[i].frame++;
    }
    frame_time++;
  }
  // show birthday name in a different color
  else if (activated & wc::BIRTHDAYS)
  {
    for (i = 0; i < SIZE; i++)
      for (j = 0; j < SIZE; j++)
        if (wc::matrix[i*SIZE+j] & (activated&wc::BIRTHDAYS))
          led_matrix.setPixelColor(idx(i,j), color.r, color.g, color.b);
  }
}

void setup()
{
  Wire.begin();
  rtc.begin();
  led_matrix.begin();

  // Adjust date and time to utc from compiletime, see Makefile. Only perform
  // when rtc is in an undefined state (i.e. battery was previously removed) or
  // when compiletime is newer
  if (!rtc.isrunning() || rtc.now().unixtime() < __UTC__)
    rtc.adjust(DateTime(__UTC__));
    
  // Set seed on unix time
  randomSeed(rtc.now().unixtime());

  // Enable all words as "bootsequence" speeding up time exponentially using
  // pretty colors
  int d = 1000;
  for (int i = 0,x,y; i < sizeof(wc::words) / sizeof(uint32_t); i++)
  {
    for (y = 0; y < SIZE; y++)
      for (x = 0; x < SIZE; x++)
        if (wc::matrix[y*SIZE+x] & wc::words[i])
          led_matrix.setPixelColor(idx(y,x), color.r, color.g, color.b);
        else
          led_matrix.setPixelColor(idx(y,x), 0, 0, 0);

    led_matrix.show();
    color = colors[i%15];
    delay(d);
    d /= 1.1;
  }
}

void loop()
{
  static uint32_t ms;
  static uint32_t activated = 0ul;
  static uint32_t previous = 0ul;
  static uint32_t tmp = 0ul;

  ms = millis();
  light_sensor.Update();
  time = time_zone.toLocal(rtc.now().unixtime());
  activated = wc::time2words(DateTime(time));

  if (activated != tmp)
  {
    reset();
    previous = tmp;
    tmp = activated;
  }
  else
  {
    ani_matrix(activated, previous);
  }
  
  led_matrix.setBrightness(light_sensor.Brightness());
  led_matrix.show();
  
  int8_t waittime = (1000/FPS) - (millis() - ms);
  if (waittime > 0)
    delay(waittime);
}

