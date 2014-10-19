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

#include "matrix.h"

/* Settings */
#define LIGHT_PIN 5 // Light sensor pin
#define B1_PIN    7 // Button 1 pin
#define B2_PIN    8 // Button 2 pin
#define LED_PIN   6 // Matrix led pin
#define WIDTH    10 // Matrix board width
#define HEIGHT   10 // Matrix board height
#define FPS      40 // Frames per second to achive

/* Macros */
#define Activate(x) activated |= x

/* Birthdays */
DateTime puck     = DateTime(2014, 3, 3);
DateTime janjelle = DateTime(1991, 5, 15);
DateTime marloes  = DateTime(1986, 5, 10);

/* Globals */
/** @brief the hardware led matrix */
Adafruit_NeoPixel led_matrix = Adafruit_NeoPixel(WIDTH * HEIGHT, LED_PIN);

/** @brief the realtime clock */
RTC_DS3231 rtc;

/** @brief convert time to list of wordmasks */
uint32_t time2words(const DateTime &time)
{
  uint32_t activated = 0;
  
  uint8_t h = time.hour() % 12;
  uint8_t m = (time.minute() + 2) / 5;

  switch (m)
  {
    case 1: case 7:  Activate(VIJF);  Activate(OVER); break;
    case 2: case 8:  Activate(TIEN);  Activate(OVER); break;
    case 3:          Activate(KWART); Activate(OVER); break;
    case 4: case 10: Activate(TIEN);  Activate(VOOR); break;
    case 5: case 11: Activate(VIJF);  Activate(VOOR); break;
    case 9:          Activate(KWART); Activate(VOOR); break;
    default: break;
  }

  if (m < 4)
    Activate(hours[h]);
  else
  if (m < 9)
  {
    Activate(HALF);
    Activate(hours[(h+1)%12]);
  }
  else
    Activate(hours[(h+1)%12]);

  if (m == 0 || m == 12)
    Activate(UUR);

  // check birthdays
  if (time.month() == puck.month() && time.year() == puck.year())
    Activate(PUCK);
  if (time.month() == marloes.month() && time.year() == marloes.year())
    Activate(MARLOES);
  if (time.month() == janjelle.month() && time.year() == janjelle.year())
    Activate(JANJELLE);

  return activated;
}

void setup()
{
  Wire.begin();
  rtc.begin();
  led_matrix.begin();

  pinMode(LIGHT_PIN, INPUT);
  pinMode(B1_PIN, INPUT);
  pinMode(B2_PIN, INPUT);

  digitalWrite(B1_PIN, LOW);
  digitalWrite(B2_PIN, LOW);  

  randomSeed(analogRead(0));
}

void loop()
{
  uint32_t ms = millis();
  DateTime time = rtc.now();
  uint32_t activated = time2words(time);
  
  for (uint8_t i = 0; i < WIDTH*HEIGHT; i++)
  {
    if (matrix[i]&activated)
      led_matrix.setPixelColor(i, 255, 0, 0);
    else
      led_matrix.setPixelColor(i, 0);
  }
  
  led_matrix.show();
  
  int32_t waittime = (1000/FPS) - (millis() - ms);
  if (waittime > 0)
    delay(waittime);
}

