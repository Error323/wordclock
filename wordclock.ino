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

/** @brief compute normal distribution given s^2 and mu=0 */
float normal(const float x, float sigma_squared)
{
  return expf(-(x*x) / (2.0f * sigma_squared));
}

uint8_t idx(int i, int j)
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
  static uint8_t wsize[SIZE]; // current frame
  static uint8_t done = 0;
  static uint8_t time = 0;
  static uint32_t prev_activated = 0ul;
  static uint32_t tmp = 0ul;
  uint8_t i, j, c;

  if (activated != tmp)
  {
    for (i = 0; i < SIZE; i++)
    {
      speed[i] = random(1, 6);
      start[i] = random(0, 10);
      wsize[i] = random(5, SIZE);
      frame[i] = 0;
    }
    done = 0;
    time = 0;
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
        if (frame[j] >= start[j] + i && frame[j] < wsize[j] + start[j] + i)
        {
          if (frame[j] > start[j]+i && wc::wordclock[i*SIZE+j] & activated)
            led_matrix.setPixelColor(idx(i,j), color);
          else
          {
            c = round(normal(frame[j]-i-start[j], wsize[j]-3) * 255);
            led_matrix.setPixelColor(idx(i,j), c, c, c);
          }
        }
        // leave (prev)activated as is, turn rest off
        else
        {
          if (wc::wordclock[i*SIZE+j] & (prev_activated|activated))
            continue;
          else
            led_matrix.setPixelColor(idx(i,j), 0);
        }
      }
    }

    // increase counters, check if done
    done = 0;
    for (uint8_t i = 0; i < SIZE; i++)
    {
      if (frame[i] > wsize[i] + start[i] + SIZE)
        done++;
      if (time % speed[i] == 0)
        frame[i]++;
    }
    time++;
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
  uint32_t ms = millis();
  light_sensor.Update();
  time = rtc.now();
  uint32_t activated = wc::time2words(time);

  ani_matrix(activated);
  
  led_matrix.setBrightness(light_sensor.Brightness());
  led_matrix.show();
  
  int32_t waittime = (1000/FPS) - (millis() - ms);
  if (waittime > 0)
    delay(waittime);
}

