#ifndef WORDCLOCK_H
#define WORDCLOCK_H

//#include <dcf77.h>

#define Activate(x) activated |= (x)

namespace wc {
//using namespace DCF77_Encoder;

#include "Matrix.h"

static const uint32_t hours[] = { TWAALF, EEN,   TWEE, DRIE,  VIER,  VIJF_,
                                  ZES,    ZEVEN, ACHT, NEGEN, TIEN_, ELF };

/** @brief convert time to wordsmask */
//uint32_t time2words(DCF77_Clock::time_t &t)
//{
//  uint32_t activated = 0ul;
//  
//  uint8_t h = BCD::bcd_to_int(t.hour) % 12;
//  uint8_t m = (BCD::bcd_to_int(t.minute) + 2) / 5;
//
//  switch (m)
//  {
//    case 1: case 7:  Activate(VIJF);  Activate(OVER); break;
//    case 2: case 8:  Activate(TIEN);  Activate(OVER); break;
//    case 3:          Activate(KWART); Activate(OVER); break;
//    case 4: case 10: Activate(TIEN);  Activate(VOOR); break;
//    case 5: case 11: Activate(VIJF);  Activate(VOOR); break;
//    case 9:          Activate(KWART); Activate(VOOR); break;
//    default: break;
//  }
//
//  if (m < 4)
//    Activate(hours[h]);
//  else
//  if (m < 9)
//  {
//    Activate(HALF);
//    Activate(hours[(h+1)%12]);
//  }
//  else
//    Activate(hours[(h+1)%12]);
//
//  if (m == 0 || m == 12)
//    Activate(UUR);
//
//  // check birthdays
//  for (uint8_t i = 0; i < sizeof(birthdays_indices)/sizeof(uint32_t); i++)
//    if (BCD::bcd_to_int(t.month) == birthdays[i].month && BCD::bcd_to_int(t.day) == birthdays[i].day)
//      Activate(birthdays_indices[i]);
//
//  return activated;
//}
}; // namespace wc

#endif // WORDCLOCK_H
