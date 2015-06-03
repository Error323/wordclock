/**
 * Wordclock with various easter eggs
 *
 * @author F. Huizinga 
 * @date Sun Oct 17:25:37 CEST 2014
 */

#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <dcf77.h>

#include "fixedptc.h"
#include "WordClock.h"
#include "LightSensor.h"
#include "Settings.h"

using namespace DCF77_Encoder;

/** @brief the color to use */
static Color color;

/** @brief the hardware led matrix */
static Adafruit_NeoPixel led_matrix(SIZE*SIZE, LED_PIN);

/** @brief the light sensor */
static LightSensor light_sensor(LDR_PIN);

/** @brief millisec counter */
static uint32_t msec = 0;
static uint32_t tsec = 0;
static bool gframe_pending = false;

/** @brief current time */
static DCF77_Clock::time_t now;

uint8_t sample_dcf77_pin()
{
  msec++;
  tsec++;
  if (msec > 1000/FPS)
  {
    gframe_pending = true;
    msec = 0;
  }
  uint8_t sample = digitalRead(DCF_PIN);
  digitalWrite(DBG_PIN, sample);
  return sample;
}

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

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  led_matrix.begin();

  // Enable debug pin
  pinMode(DBG_PIN, OUTPUT);

  // Set seed
  randomSeed(analogRead(LDR_PIN));

  // Turn all leds off
  for (uint8_t i = 0; i < SIZE*SIZE; i++)
    led_matrix.setPixelColor(i, 0);
  led_matrix.show();

  // Initialize dcf77 radio clock
  DCF77_Clock::setup();
  DCF77_Clock::set_input_provider(sample_dcf77_pin);

  // Wait till clock is synced, depending on the signal quality this may take
  // rather long. About 5 minutes with a good signal, 30 minutes or longer
  // with a bad signal
  uint8_t counter = 0;
  for (uint8_t state = DCF77::useless;
       state == DCF77::useless || state == DCF77::dirty;
       state = DCF77_Clock::get_clock_state())
  {
    // wait for next sec
    DCF77_Clock::get_current_time(now);
    Serial.print(state);
    counter++;
    if (counter >= 60) 
    {   
      Serial.print(" ");
      Serial.print(DCF77_Second_Decoder::get_prediction_match());
      Serial.println();
      counter = 0;
    }   
    else
    if (counter % 5 == 0)
      Serial.print(" ");
  }

  Serial.print("Synced in ");
  Serial.print(tsec);
  Serial.print(" ms");
}

void loop()
{
  static uint32_t activated = 0ul;
  static uint32_t previous = 0ul;
  static uint32_t tmp = 0ul;
  DCF77_Clock::get_current_time(now);

  light_sensor.Update();
  activated = wc::time2words(now);

  if (activated != tmp)
  {
    reset();
    previous = tmp;
    tmp = activated;
    animate(activated, previous, light_sensor.Brightness());
  }
}

