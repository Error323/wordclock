/**
 * Wordclock with various easter eggs
 *
 * @author F. Huizinga 
 * @date Sun Oct 17:25:37 CEST 2014
 */

#include <Wire.h>
#include <Time.h>

#include <DCF77.h>
#include <Adafruit_NeoPixel.h>

#include "fixedptc.h"
#include "WordClock.h"
#include "LightSensor.h"

/* Settings */
#define LIGHT_PIN 0 // Light sensor pin
#define LED_PIN   3 // Matrix led pin
#define DCF_PIN   2 // Radio clock pin
#define DCF_INT   0 // Radio clock interrupt
#define SIZE     10 // Matrix board size x size
#define FPS      40 // Frames per second to achieve

#define idx(i,j) ((i)*SIZE+(j))

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

static Color color;

/** @brief the hardware led matrix */
static Adafruit_NeoPixel led_matrix(SIZE*SIZE, LED_PIN);

/** @brief the radio clock */
static DCF77 dcf = DCF77(DCF_PIN, DCF_INT);

/** @brief the light sensor */
static LightSensor light_sensor(LIGHT_PIN);

void reset()
{
}

void animate(const uint32_t activated, const uint32_t previous, const uint8_t brightness)
{
  uint8_t i, j;
  for (i = 0; i < SIZE; i++)
    for (j = 0; j < SIZE; j++)
      if (wc::matrix[idx(i,j)] & activated)
        led_matrix.setPixelColor(idx(i,j), brightness);
      else
        led_matrix.setPixelColor(idx(i,j), 0);
}

time_t getDCFTime()
{ 
  time_t t = dcf.getTime();
  return t;
}

void setup()
{
  Serial.begin(9600);
  Serial.println("1   - binary 1 corresponding to long pulse");
  Serial.println("0   - binary 0 corresponding to short pulse");
  Serial.println("BF  - Buffer is full at end of time-sequence. This is good");
  Serial.println("EoB - Buffer is full before at end of time-sequence");
  Serial.println("EoM - Buffer is not yet full at end of time-sequence");
  Wire.begin();
  led_matrix.begin();
  dcf.Start();
  setSyncInterval(30);
  setSyncProvider(getDCFTime);

  // Wait for time update
  for (uint8_t i = 0; i < SIZE*SIZE; i++)
    led_matrix.setPixelColor(i, 0);
  led_matrix.show();

  while (timeStatus() == timeNotSet)
    delay(1000);

  // Set seed
  randomSeed(analogRead(LIGHT_PIN));
}

void loop()
{
  static uint32_t ms;
  static uint32_t activated = 0ul;
  static uint32_t previous = 0ul;
  static uint32_t tmp = 0ul;

  ms = millis();
  light_sensor.Update();
  activated = wc::time2words();

  if (activated != tmp)
  {
    reset();
    previous = tmp;
    tmp = activated;
  }
  else
  {
    animate(activated, previous, light_sensor.Brightness());
  }
  
  led_matrix.show();
  
  int8_t waittime = (1000/FPS) - (millis() - ms);
  if (waittime > 0)
    delay(waittime);
}

