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

#include "WordClock.h"
#include "LightSensor.h"

/* Settings */
#define LIGHT_PIN 0 // Light sensor pin
#define B1_PIN    6 // Button 1 pin
#define B2_PIN    8 // Button 2 pin
#define LED_PIN   7 // Matrix led pin
#define SIZE     10 // Matrix board size x size
#define FPS      40 // Frames per second to achieve

/* Globals */
static uint32_t color = Adafruit_NeoPixel::Color(200, 200, 200);

/** @brief the hardware led matrix */
static Adafruit_NeoPixel led_matrix(SIZE*SIZE, LED_PIN);

/** @brief the realtime clock */
static RTC_DS3231 rtc;

/** @brief the light sensor */
static LightSensor light_sensor(LIGHT_PIN);

/** @brief the current time */
static DateTime time;

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
            led_matrix.setPixelColor(idx(i,j), color);
          else
          {
            c = round(normal(L[j].frame-i-L[j].start, L[j].asize-3) * 255);
            led_matrix.setPixelColor(idx(i,j), c, c, c);
          }
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
      if (L[j].frame > L[j].asize + L[j].start + SIZE)
        done++;
      if (frame_time % L[j].speed == 0)
        L[j].frame++;
    }
    frame_time++;
  }
}

void setup()
{
  Wire.begin();
  rtc.begin();
  led_matrix.begin();
  Serial.begin(9600);

  pinMode(B1_PIN, INPUT);
  pinMode(B2_PIN, INPUT);

  digitalWrite(B1_PIN, LOW);
  digitalWrite(B2_PIN, LOW);  

  if (!rtc.isrunning())
    rtc.adjust(DateTime(__DATE__, __TIME__));
    
  time = rtc.now();
  randomSeed(time.unixtime());
}

void loop()
{
  static uint32_t ms;
  static uint32_t activated = 0ul;
  static uint32_t previous = 0ul;
  static uint32_t tmp = 0ul;

  ms = millis();
  light_sensor.Update();
  time = rtc.now();
  activated = wc::time2words(time);

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

