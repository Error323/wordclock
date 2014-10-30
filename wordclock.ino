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

#include "MatrixAnimation.h"
#include "NexusAnimation.h"
#include "BirthdayAnimation.h"

/* Settings */
#define LIGHT_PIN 0 // Light sensor pin
#define B1_PIN    6 // Button 1 pin
#define B2_PIN    8 // Button 2 pin
#define LED_PIN   7 // Matrix led pin
#define SIZE     10 // Matrix board size x size
#define FPS      40 // Frames per second to achieve

/* Globals */
/** @brief the hardware led matrix */
static Adafruit_NeoPixel led_matrix(SIZE*SIZE, LED_PIN);

/** @brief pointers to all animations */
static Animation *animations[] = { nullptr, nullptr, nullptr };

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

  animations[0] = new MatrixAnimation(led_matrix.getPixels()); 
  animations[1] = new NexusAnimation(led_matrix.getPixels()); 
  animations[2] = new BirthdayAnimation(led_matrix.getPixels()); 
}

void loop()
{
  static uint32_t active = 0ul;
  static uint32_t prev = 0ul;
  static uint32_t tmp = 0ul;
  static uint32_t ms;
  static Animation *animation = nullptr;

  ms = millis();
  time = rtc.now();
  light_sensor.Update();
  active = wc::time2words(time);

  // New time, reset
  if (active != tmp)
  {
    if (active&BIRTHDAYS)
      animation = animations[2];
    else
      animation = animations[random(2)];

    animation->Reset();
    prev = tmp;
    tmp = active;
  }
  // Continue animation
  else if (!animation->IsDone())
    animation->Tick(active, prev);

  // Set the brightness
  led_matrix.setBrightness(light_sensor.Brightness());

  // Send data to the leds
  led_matrix.show();
  
  // Maintain steady fps
  int8_t waittime = (1000/FPS) - (millis() - ms);
  if (waittime > 0)
    delay(waittime);
}

