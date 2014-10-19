/**
 * J K W A R T I E N X 
 * A V I J F M V O O R 
 * N O V E R A H A L F 
 * J H T H D R I E V N 
 * E T W A A L F P I E 
 * L A E Z Z O Y U J G 
 * L C E E E E N C F E 
 * E H Q V Z S D K S N 
 * C T I E N V I E R M 
 * E L F N Q C I U U R 
 */

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

static const uint32_t matrix[] = {
  0x80000, 0x4,      0x4,     0x4,      0x4,     0x10004,  0x10000, 0x10000,
  0x10000, 0x0,      0x80000, 0x800,    0x800,   0x800,    0x800,   0x100000,
  0x8,     0x8,      0x8,     0x8,      0x80000, 0x10,     0x10,    0x10,
  0x10,    0x100000, 0x20,    0x20,     0x20,    0x20,     0x80000, 0x0,
  0x100,   0x0,      0x200,   0x100200, 0x200,   0x200,    0x0,     0x8000,
  0x80000, 0x40,     0x140,   0x40,     0x40,    0x100040, 0x40,    0x200000,
  0x0,     0x8000,   0x80000, 0x4000,   0x100,   0x3000,   0x0,     0x100000,
  0x0,     0x200000, 0x0,     0x8000,   0x80000, 0x4000,   0x100,   0x2000,
  0x1080,  0x100080, 0x80,    0x200000, 0x0,     0x8000,   0x80000, 0x4000,
  0x0,     0x2000,   0x0,     0x101000, 0x0,     0x200000, 0x0,     0x8000,
  0x0,     0x4000,   0x0,     0x2000,   0x0,     0x400,    0x400,   0x400,
  0x400,   0x0,      0x20000, 0x20000,  0x20000, 0x2000,   0x0,     0x0,
  0x0,     0x40000,  0x40000, 0x40000
};

