/**
 * Wordclock with various easter eggs
 *
 * @author F. Huizinga
 * @date Sun Oct 17:25:37 CEST 2014
 */

#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <avr/pgmspace.h>
#include <dcf77.h>

#include "fixedptc.h"
#include "WordClock.h"
#include "LightSensor.h"
#include "Settings.h"

using namespace DCF77_Encoder;

/** @brief the hardware led matrix */
static Adafruit_NeoPixel led_matrix(SIZE*SIZE, LED_PIN);

/** @brief the light sensor */
static LightSensor light_sensor(LDR_PIN);

/** @brief millisec counter */
static volatile uint16_t msec = 0;
static volatile bool gframe_pending = false;

/** @brief current time */
static DCF77_Clock::time_t now;

uint8_t sample_dcf77_pin()
{
  msec++;
  if (msec > 1000 / FPS)
  {
    gframe_pending = true;
    msec = 0;
  }
  uint8_t sample = digitalRead(DCF_PIN);
  digitalWrite(DBG_PIN, sample);
  return sample;
}

struct point_t
{
  fixedpt x;
  fixedpt y;
};

template <typename T> void shuffle(T *L, uint8_t n)
{
  uint8_t j;
  for (uint8_t i = 0; i < n - 1; i++)
  {
    j = random(i, n);
    T t = L[i];
    L[i] = L[j];
    L[j] = t;
  }
}

template <typename T> fixedpt sign(T v)
{
  return fixedpt_fromint((v > 0) - (v < 0));
}

uint8_t idx(uint8_t i, uint8_t j)
{
  if (i & 1)
    j = SIZE - j - 1;

  return (SIZE - i - 1) * SIZE + (SIZE - j - 1);
}

void clear(point_t &p, uint32_t activated)
{
  uint8_t i;
  uint32_t w;
  for (fixedpt y = fixedpt_floor(p.y); y <= fixedpt_ceil(p.y); y += FIXEDPT_ONE)
  {
    for (fixedpt x = fixedpt_floor(p.x); x <= fixedpt_ceil(p.x);
         x += FIXEDPT_ONE)
    {
      i = idx(fixedpt_toint(y), fixedpt_toint(x));
      w = pgm_read_dword(&wc::matrix[i]);
      if (w & activated)
        continue;
      led_matrix.setPixelColor(i, 0ul);
    }
  }
}

void plot_aa(point_t &p, uint8_t b)
{
  fixedpt pct_x, pct_y, pct;
  uint8_t i;
  uint32_t v;
  for (fixedpt y = fixedpt_floor(p.y); y <= fixedpt_ceil(p.y); y += FIXEDPT_ONE)
  {
    for (fixedpt x = fixedpt_floor(p.x); x <= fixedpt_ceil(p.x);
         x += FIXEDPT_ONE)
    {
      pct_x = FIXEDPT_ONE - fixedpt_abs(p.x - x);
      pct_y = FIXEDPT_ONE - fixedpt_abs(p.y - y);
      pct = fixedpt_mul(pct_x, pct_y);
      i = idx(fixedpt_toint(y), fixedpt_toint(x));
      v = fixedpt_toint(fixedpt_round(fixedpt_mul(fixedpt_fromint(b), pct)));
      v = (v << 16 | v << 8 | v);
      led_matrix.setPixelColor(i, v | led_matrix.getPixelColor(i));
    }
  }
}

void wait(const uint8_t duration)
{
  uint16_t frame = 0;
  while (frame < duration*FPS)
  {
    if (gframe_pending)
    {
      gframe_pending = false;
      frame++;
    }
  }
}

void ani_sparkle(const uint32_t activated, const uint8_t duration)
{
  uint8_t ids[32];
  uint8_t start[32];
  uint8_t n, i;

  n = 0;
  for (i = 0; i < SIZE*SIZE; i++)
  {
    uint32_t w = pgm_read_dword(&wc::matrix[i]);
    // if activated and odds in our favor
    if (w & activated && random(10) == 0)
    {
      ids[n] = i;
      start[n] = random(duration*FPS - FPS);
      n++;
    }
  }

  uint16_t frame = 0;
  uint32_t t, c, v;
  fixedpt pct;
  while (frame < duration * FPS)
  {
    if (gframe_pending)
    {
      gframe_pending = false;
      for (i = 0; i < n; i++)
      {
        t = start[i] - frame;
        if (t >= 0 && t < FPS)
        {
          pct = FIXEDPT_ONE - fixedpt_div(fixedpt_fromint(t), fixedpt_fromint(FPS));
          v = fixedpt_toint(fixedpt_mul(pct, fixedpt_fromint(255)));
          c = led_matrix.getPixelColor(ids[i]);
          c |= (v<<16|v<<8|v);
          led_matrix.setPixelColor(ids[i], c);
        }
      }
      frame++;
      led_matrix.show();
    }
  }
}

void ani_flow(const uint32_t activated, const uint32_t previous,
             const uint8_t brightness, const uint8_t duration)
{
  point_t src[32];
  point_t dst[32];
  fixedpt spd[32];
  uint8_t n, m, i, j;
  n = m = 0;

  // 1. Determine sources and destinations
  for (i = 0; i < SIZE; i++)
  {
    for (j = 0; j < SIZE; j++)
    {
      uint32_t w = pgm_read_dword(&wc::matrix[idx(i, j)]);
      if (w & previous)
        src[n++] = point_t{ fixedpt_fromint(j), fixedpt_fromint(i) };
      else if (w & activated)
        dst[m++] = point_t{ fixedpt_fromint(j), fixedpt_fromint(i) };
    }
  }

  // 2. Shuffle sources for some randomness in animations
  shuffle(src, n);

  // 3. Each source has one or more destinations or each destination one or
  // more sources
  if (n > m)
    for (i = m; i < n; i++)
      dst[i] = dst[random(m)];
  else
    for (i = n; i < m; i++)
      src[i] = src[random(n)];

  // 4. Compute some usefull stats
  n = max(n, m);

  point_t p;
  fixedpt dist;
  int max_norm;
  for (i = 0; i < n; i++)
  {
    p = point_t{ dst[i].x - src[i].x, dst[i].y - src[i].y };
    dist = fixedpt_abs(p.x) + fixedpt_abs(p.y);
    max_norm = random(fixedpt_toint(dist), duration*FPS) / fixedpt_toint(dist);
    spd[i] = fixedpt_div(FIXEDPT_ONE, fixedpt_fromint(max_norm));
    Serial.println(fixedpt_cstr(spd[i], 3));
  }

  // 5. Animate
  uint16_t frame = 0;
  while (frame < duration * FPS)
  {
    if (gframe_pending)
    {
      gframe_pending = false;
      for (i = 0; i < n; i++)
      {
        if (fixedpt_abs(src[i].x - dst[i].x) <= spd[i] &&
            fixedpt_abs(src[i].y - dst[i].y) <= spd[i])
          continue;

        p = point_t{ dst[i].x - src[i].x, dst[i].y - src[i].y };
        if (fixedpt_abs(p.x) > fixedpt_abs(p.y))
          src[i].x += fixedpt_mul(sign(p.x), spd[i]);
        else
          src[i].y += fixedpt_mul(sign(p.y), spd[i]);
        plot_aa(src[i], brightness);
      }
      frame++;
      led_matrix.show();
      for (i = 0; i < n; i++)
        clear(src[i], activated);
    }
  }
  for (i = 0; i < n; i++)
    clear(src[i], activated);
  led_matrix.show();
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
  for (uint8_t i = 0; i < SIZE * SIZE; i++)
    led_matrix.setPixelColor(i, 0);
  led_matrix.show();

  // Initialize dcf77 radio clock
  DCF77_Clock::setup();
  DCF77_Clock::set_input_provider(sample_dcf77_pin);

  ani_flow(wc::RING|wc::WYKE|wc::HAJO, wc::KWART|wc::VOOR|wc::VIER, 255, 3);
  wait(4);
  ani_flow(wc::TIEN|wc::VOOR|wc::VIER, wc::RING|wc::WYKE|wc::HAJO, 255, 3);

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
      Serial.print(F(" "));
      Serial.print(DCF77_Second_Decoder::get_prediction_match());
      Serial.println();
      counter = 0;
    }
    else
    if (counter % 5 == 0)
      Serial.print(F(" "));
  }
  Serial.println();
}

void loop()
{
  static uint32_t activated = 0ul;
  static uint32_t previous = 0ul;
  static uint32_t tmp = wc::UUR;
  DCF77_Clock::get_current_time(now);

  light_sensor.Update();
  activated = wc::time2words(now);

  if (activated != tmp)
  {
    previous = tmp;
    tmp = activated;
    ani_flow(wc::RING|wc::WYKE|wc::HAJO, previous, light_sensor.Brightness(), 3);
    //ani_sparkle(wc::RING|wc::WYKE|wc::HAJO, 4);
    wait(4);
    ani_flow(activated, wc::RING|wc::WYKE|wc::HAJO, light_sensor.Brightness(), 3);
  }
}

