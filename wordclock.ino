/**
 * Wordclock with various easter eggs
 *
 * @author F. Huizinga 
 * @date Sun Oct 17:25:37 CEST 2014
 */

#include <Wire.h>
#include <Adafruit_NeoPixel.h>
//#include <dcf77.h>

#include "fixedptc.h"
#include "WordClock.h"
#include "LightSensor.h"
#include "Settings.h"

//using namespace DCF77_Encoder;

/** @brief the color to use */
static Color color;

/** @brief the hardware led matrix */
static Adafruit_NeoPixel led_matrix(SIZE*SIZE, LED_PIN);

/** @brief the light sensor */
static LightSensor light_sensor(LDR_PIN);

/** @brief millisec counter */
//static uint32_t msec = 0;
//static uint32_t tsec = 0;
//static bool gframe_pending = false;
//
///** @brief current time */
//static DCF77_Clock::time_t now;
//
//uint8_t sample_dcf77_pin()
//{
//  msec++;
//  tsec++;
//  if (msec > 1000/FPS)
//  {
//    gframe_pending = true;
//    msec = 0;
//  }
//  uint8_t sample = digitalRead(DCF_PIN);
//  digitalWrite(DBG_PIN, sample);
//  return sample;
//}

struct point_t {
  fixedpt x;
  fixedpt y;
};

template<typename T>
void shuffle(T *L, uint8_t n)
{
  uint8_t j;
  for (uint8_t i = 0; i < n-1; i++)
  {
    j = random(i, n);
    T t = L[i];
    L[i] = L[j];
    L[j] = t;
  }
}

template<typename T>
fixedpt sign(T v) {
  return fixedpt_fromint((v > 0) - (v < 0));
}

int idx(int i, int j)
{
  if (i & 1)
    j = SIZE - 1 - j;

  return (SIZE-i-1)*SIZE + (SIZE-j-1);
}

void clear(point_t &p, uint32_t activated)
{
  for (fixedpt y = fixedpt_floor(p.y); y <= fixedpt_ceil(p.y); y+=fixedpt_fromint(1))
  {
    for (fixedpt x = fixedpt_floor(p.x); x <= fixedpt_ceil(p.x); x+=fixedpt_fromint(1))
    {
      fixedpt pct_x = fixedpt_fromint(1) - fixedpt_abs(p.x - x);
      fixedpt pct_y = fixedpt_fromint(1) - fixedpt_abs(p.y - y);
      fixedpt pct = fixedpt_mul(pct_x, pct_y);
      int i = idx(fixedpt_toint(y), fixedpt_toint(x));
      if (wc::matrix[i] & activated)
        continue;
      led_matrix.setPixelColor(i, 0ul);
    }
  }
}

void plot_aa(point_t &p, uint8_t b)
{
  for (fixedpt y = fixedpt_floor(p.y); y <= fixedpt_ceil(p.y); y+=fixedpt_fromint(1))
  {
    for (fixedpt x = fixedpt_floor(p.x); x <= fixedpt_ceil(p.x); x+=fixedpt_fromint(1))
    {
      fixedpt pct_x = fixedpt_fromint(1) - fixedpt_abs(p.x - x);
      fixedpt pct_y = fixedpt_fromint(1) - fixedpt_abs(p.y - y);
      fixedpt pct = fixedpt_mul(pct_x, pct_y);
      int i = idx(fixedpt_toint(y), fixedpt_toint(x));
      uint32_t v = fixedpt_toint(fixedpt_round(fixedpt_mul(fixedpt_fromint(b), pct)));
      v = (v<<16|v<<8|v);
      uint32_t u = led_matrix.getPixelColor(i);
      led_matrix.setPixelColor(i, v|u);
    }
  }
}

void animate(const uint32_t activated, const uint32_t previous, const uint8_t brightness)
{
  point_t src[32];
  point_t dst[32];
  fixedpt spd[32];
  uint8_t n,m,i,j;
  n = m = 0;

  // 1. Determine sources and destinations
  Serial.println("Determine");
  for (i = 0; i < SIZE; i++)
    for (j = 0; j < SIZE; j++)
      if (wc::matrix[idx(i,j)] & previous)
        src[n++] = point_t{fixedpt_fromint(j), fixedpt_fromint(i)};
      else
      if (wc::matrix[idx(i,j)] & activated)
        dst[m++] = point_t{fixedpt_fromint(j), fixedpt_fromint(i)};

  // 2. Shuffle sources for some randomness in animations
  Serial.println("Shuffle");
  shuffle(src, n);

  // 3. Each source has one or more destinations or each destination one or
  // more sources
  Serial.println("Equalize");
  if (n > m)
    for (i = m; i < n; i++)
      dst[i] = dst[random(m)];
  else
    for (i = n; i < m; i++)
      src[i] = src[random(n)];

  // 4. Compute some usefull stats
  Serial.println("Stats");
  n = max(n, m);
  
  point_t p;
  fixedpt dist;
  for (i = 0; i < n; i++)
  {
    p = point_t{dst[i].x - src[i].x, dst[i].y - src[i].y};
    dist = fixedpt_abs(p.x) + fixedpt_abs(p.y);
    spd[i] = fixedpt_div(dist, fixedpt_fromint(random(fixedpt_toint(dist), 3 * FPS + 1)));
  }

  // 5. Animate
  Serial.println("Animate");
  uint16_t frame = 0;
  while (frame < 3*FPS)
  {
    uint32_t ms = millis();
    for (i = 0; i < n; i++)
    {
      if (fixedpt_abs(src[i].x - dst[i].x) < spd[i] && fixedpt_abs(src[i].y - dst[i].y) < spd[i])
        continue;

      p = point_t{dst[i].x - src[i].x, dst[i].y - src[i].y};
      if (fixedpt_abs(p.x) > fixedpt_abs(p.y))
        src[i].x += fixedpt_mul(sign(p.x), spd[i]);
      else
        src[i].y += fixedpt_mul(sign(p.y), spd[i]);
      plot_aa(src[i], 255);
    }
    frame++;
    led_matrix.show();
    for (i = 0; i < n; i++)
      clear(src[i], activated);
    int32_t waittime = (1000/FPS) - (millis() - ms);
    if (waittime > 0)
      delay(waittime);
  }
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

  delay(1000);
  animate(wc::NEGEN, wc::KWART, 255);

  // Initialize dcf77 radio clock
  /*
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

  Serial.println("");
  Serial.print("Synced in ");
  Serial.print(tsec);
  Serial.println(" ms");
  */
}

void loop()
{
/*
  static uint32_t activated = 0ul;
  static uint32_t previous = 0ul;
  static uint32_t tmp = 0ul;
  //DCF77_Clock::get_current_time(now);

  light_sensor.Update();
  activated = wc::time2words(now);

  if (activated != tmp)
  {
    previous = tmp;
    tmp = activated;
    animate(activated, previous, light_sensor.Brightness());
  }
*/
}

