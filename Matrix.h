/**
 * Autogenerated header file for the wordclock
 *   23 words encoded
 *   14/103 empty squares
 *
 * K W A R T I E N F R 
 * W V I J F A O V E R 
 * Z V O O R J H A L F 
 * A Y Z E S A Z N I T 
 * N C Q E E N T E C W 
 * M V H N V J I G I A 
 * P I I T W E E E A A 
 * U J N E T L N N L L 
 * C F M A R L O E S F 
 * K Q D R I E T U U R 
 */

static const uint32_t VIJF       = (1ul << 0);
static const uint32_t TIEN       = (1ul << 1);
static const uint32_t KWART      = (1ul << 2);
static const uint32_t VOOR       = (1ul << 3);
static const uint32_t OVER       = (1ul << 4);
static const uint32_t HALF       = (1ul << 5);
static const uint32_t TWAALF     = (1ul << 6);
static const uint32_t EEN        = (1ul << 7);
static const uint32_t TWEE       = (1ul << 8);
static const uint32_t DRIE       = (1ul << 9);
static const uint32_t VIER       = (1ul << 10);
static const uint32_t VIJF_      = (1ul << 11);
static const uint32_t ZES        = (1ul << 12);
static const uint32_t ZEVEN      = (1ul << 13);
static const uint32_t ACHT       = (1ul << 14);
static const uint32_t NEGEN      = (1ul << 15);
static const uint32_t TIEN_      = (1ul << 16);
static const uint32_t ELF        = (1ul << 17);
static const uint32_t UUR        = (1ul << 18);
static const uint32_t JANJELLE   = (1ul << 19);
static const uint32_t MARLOES    = (1ul << 20);
static const uint32_t PUCK       = (1ul << 21);
static const uint32_t FELICIA    = (1ul << 22);
static const uint32_t HORIZONTAL = (1ul << 29);
static const uint32_t VERTICAL   = (1ul << 30);
static const uint32_t DIAGONAL   = (1ul << 31);
static const uint32_t BIRTHDAYS = PUCK | JANJELLE | MARLOES | FELICIA;

static const uint32_t hours[] = { TWAALF, EEN,   TWEE, DRIE,  VIER,  VIJF_,
                                  ZES,    ZEVEN, ACHT, NEGEN, TIEN_, ELF };

static const uint32_t words[] = { VIJF,    TIEN,   KWART,  VOOR,  OVER,
                                  HALF,    TWAALF, EEN,    TWEE,  DRIE,
                                  VIER,    VIJF_,  ZES,    ZEVEN, ACHT,
                                  NEGEN,   TIEN_,  ELF,    UUR,   JANJELLE,
                                  MARLOES, PUCK,   FELICIA };

static const uint32_t matrix[] = {
  536870916,  536870916,  536870916,  536870916,  536870918,  536870914,
  536870914,  536870914,  1077936128, 0,          0,          536870913,
  536870913,  536870913,  536870913,  0,          536870928,  536870928,
  1614807056, 536870928,  0,          536870920,  536870920,  536870920,
  536870920,  1074266112, 536870944,  536870944,  1614807072, 536870944,
  2147500032, 0,          2684366848, 1610616960, 536875008,  1074266112,
  0,          1073774592, 1077936128, 1073741888, 0,          2147500032,
  0,          3221233792, 0,          1074266112, 1073807360, 1073774592,
  1077936128, 1073741888, 0,          3221228544, 2147500032, 1073741952,
  2147491840, 1074266112, 1073807360, 1073774592, 1077936128, 1073741888,
  1075838976, 1073743872, 2147484672, 2684371200, 536871168,  3758629120,
  1610678528, 3221389312, 1077936128, 1073741888, 1075838976, 1073743872,
  0,          2147484672, 0,          1074266112, 3221299200, 1073774592,
  2147614720, 1073741888, 1075838976, 1073743872, 537919488,  537919488,
  2685404160, 1612185600, 537919488,  537919488,  537919488,  3221356608,
  1075838976, 0,          536871424,  536871424,  536871424,  1611137536,
  0,          537133056,  537133056,  537133056
};

