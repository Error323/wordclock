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
#define LIGHT_PIN 5 // Light sensor pin
#define B1_PIN    7 // Button 1 pin
#define B2_PIN    8 // Button 2 pin
#define LED_PIN   6 // Matrix led pin
#define SIZE     10 // Matrix board size x size
#define FPS      40 // Frames per second to achieve

/* Globals */
/** @brief the hardware led matrix */
static Adafruit_NeoPixel led_matrix(SIZE*SIZE, LED_PIN);

/** @brief the realtime clock */
static RTC_DS3231 rtc;

/** @brief the light sensor */
static LightSensor light_sensor(LIGHT_PIN);

/** @brief the current time */
static DateTime time;

void setup()
{
  Wire.begin();
  rtc.begin();
  led_matrix.begin();

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
  
  for (uint8_t i = 0; i < SIZE*SIZE; i++)
  {
    if (wc::wordclock[i]&activated)
      led_matrix.setPixelColor(i, 255, 0, 0);
    else
      led_matrix.setPixelColor(i, 0);
  }
  
  led_matrix.setBrightness(light_sensor.Brightness());
  led_matrix.show();
  
  int32_t waittime = (1000/FPS) - (millis() - ms);
  if (waittime > 0)
    delay(waittime);
}

