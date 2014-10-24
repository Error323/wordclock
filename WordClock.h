/**
 * Autogenerated header file for the wordclock
 *   22 words encoded
 *   20/96 empty squares
 *
 * F K W A R T I E N D 
 * V I J F A W O V E R 
 * V O O R J H H A L F 
 * T N Q M A R L O E S 
 * T W E E N Z V I J F 
 * P Z A G J Z W I Z C 
 * U E A A E D R I E J 
 * C V C J L N C H S R 
 * K E H E L F Y M V P 
 * H N T I E N M U U R 
 */

#include <RTClib.h>

namespace wc {
  
/* Birthdays */
DateTime puck     = DateTime(2014, 3, 3);
DateTime janjelle = DateTime(1991, 5, 15);
DateTime marloes  = DateTime(1986, 5, 10);

/* Macros */
#define Activate(x) activated |= x

static const uint32_t VIJF     = (1ul << 0);
static const uint32_t TIEN     = (1ul << 1);
static const uint32_t KWART    = (1ul << 2);
static const uint32_t VOOR     = (1ul << 3);
static const uint32_t OVER     = (1ul << 4);
static const uint32_t HALF     = (1ul << 5);
static const uint32_t TWAALF   = (1ul << 6);
static const uint32_t EEN      = (1ul << 7);
static const uint32_t TWEE     = (1ul << 8);
static const uint32_t DRIE     = (1ul << 9);
static const uint32_t VIER     = (1ul << 10);
static const uint32_t VIJF_    = (1ul << 11);
static const uint32_t ZES      = (1ul << 12);
static const uint32_t ZEVEN    = (1ul << 13);
static const uint32_t ACHT     = (1ul << 14);
static const uint32_t NEGEN    = (1ul << 15);
static const uint32_t TIEN_    = (1ul << 16);
static const uint32_t ELF      = (1ul << 17);
static const uint32_t UUR      = (1ul << 18);
static const uint32_t JANJELLE = (1ul << 19);
static const uint32_t MARLOES  = (1ul << 20);
static const uint32_t PUCK     = (1ul << 21);

static const uint32_t hours[] = { TWAALF, EEN,   TWEE, DRIE,  VIER,  VIJF_,
                                  ZES,    ZEVEN, ACHT, NEGEN, TIEN_, ELF };

static const uint32_t wordclock[] = {
  0,       4,       4,       4,       4,       65540,   65536,   65536,
  65536,   0,       2048,    2048,    2048,    2048,    0,       0,
  16,      16,      16,      16,      8,       8,       8,       8,
  524288,  0,       32,      32,      32,      32,      64,      32768,
  0,       1048576, 1572864, 1048576, 1048576, 1048576, 1048576, 1048576,
  256,     320,     33152,   384,     524416,  0,       1024,    0,
  0,       0,       2097152, 8192,    64,      32768,   524288,  0,
  0,       1024,    4096,    0,       2097152, 8192,    16384,   64,
  557056,  512,     512,     512,     5632,    0,       2097152, 8192,
  16384,   0,       524352,  32768,   0,       0,       4096,    1024,
  2097152, 8192,    16384,   131072,  655360,  131136,  0,       0,
  0,       0,       0,       8192,    16384,   0,       524288,  0,
  0,       262144,  262144,  262144
};

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
  if (time.month() == puck.month() && time.day() == puck.day())
    Activate(PUCK);
  if (time.month() == marloes.month() && time.day() == marloes.day())
    Activate(MARLOES);
  if (time.month() == janjelle.month() && time.day() == janjelle.day())
    Activate(JANJELLE);

  return activated;
}
}; // namespace wc