/**
 * J K W A R T I E N V 
 * A V I J F M V O O R 
 * N O V E R A H A L F 
 * J U T P D R I E V N 
 * E T W A A L F P I E 
 * L A E Z Z O A U J G 
 * L C E E N E N C F E 
 * E H Y V W S S K C N 
 * C T I E N V I E R X 
 * E L F N Q G X U U R 
 */

#define VIJF      (1ul << 0)
#define TIEN      (1ul << 1)
#define KWART     (1ul << 2)
#define VOOR      (1ul << 3)
#define OVER      (1ul << 4)
#define HALF      (1ul << 5)
#define TWAALF    (1ul << 6)
#define EEN       (1ul << 7)
#define TWEE      (1ul << 8)
#define DRIE      (1ul << 9)
#define VIER      (1ul << 10)
#define VIJF_     (1ul << 11)
#define ZES       (1ul << 12)
#define ZEVEN     (1ul << 13)
#define ACHT      (1ul << 14)
#define NEGEN     (1ul << 15)
#define TIEN_     (1ul << 16)
#define ELF       (1ul << 17)
#define UUR       (1ul << 18)
#define JANJELLE  (1ul << 19)
#define MARLOES   (1ul << 20)
#define PUCK      (1ul << 21)

static const uint32_t hours[] = { TWAALF, EEN,   TWEE, DRIE,  VIER,  VIJF_,
                                  ZES,    ZEVEN, ACHT, NEGEN, TIEN_, ELF };

static const uint32_t matrix[] = {
  0x80000, 0x4,      0x4,     0x4,      0x4,     0x10004,  0x10000, 0x10000,
  0x10000, 0x0,      0x80000, 0x800,    0x800,   0x800,    0x800,   0x100000,
  0x8,     0x8,      0x8,     0x8,      0x80000, 0x10,     0x10,    0x10,
  0x10,    0x100000, 0x20,    0x20,     0x20,    0x20,     0x80000, 0x0,
  0x100,   0x0,      0x200,   0x100200, 0x200,   0x200,    0x0,     0x8000,
  0x80000, 0x40,     0x140,   0x40,     0x40,    0x100040, 0x40,    0x200000,
  0x0,     0x8000,   0x80000, 0x4000,   0x100,   0x2000,   0x1000,  0x100000,
  0x0,     0x200000, 0x0,     0x8000,   0x80000, 0x4000,   0x180,   0x2080,
  0x80,    0x101000, 0x0,     0x200000, 0x0,     0x8000,   0x80000, 0x4000,
  0x0,     0x2000,   0x0,     0x100000, 0x1000,  0x200000, 0x0,     0x8000,
  0x0,     0x4000,   0x0,     0x2000,   0x0,     0x400,    0x400,   0x400,
  0x400,   0x0,      0x20000, 0x20000,  0x20000, 0x2000,   0x0,     0x0,
  0x0,     0x40000,  0x40000, 0x40000
};

