#ifndef WORDCLOCK_H
#define WORDCLOCK_H

#include <RTClib.h>

#define Activate(x) activated |= (x)

namespace wc {

#include "Matrix.h"

/* Birthdays */
static const DateTime bdays[] = { DateTime(2014, 3, 3),
                                      DateTime(1991, 5, 15),
                                      DateTime(1986, 5, 10),
                                      DateTime(2010, 8, 22) };

static const uint32_t bdays_idx[] = { PUCK, JANJELLE, MARLOES, FELICIA };
static const uint32_t BIRTHDAYS = JANJELLE | MARLOES | PUCK | FELICIA;

/** @brief convert time to wordsmask */
uint32_t time2words(const DateTime &time)
{
  uint32_t activated = 0ul;
  
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
  for (uint8_t i = 0; i < sizeof(bdays_idx)/sizeof(uint32_t); i++)
    if (time.month() == bdays[i].month() && time.day() == bdays[i].day())
      Activate(bdays_idx[i]);

  return activated;
}
}; // namespace wc

#endif // WORDCLOCK_H
