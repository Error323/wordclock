/**
 * Auto generated using scripts/mat2hdr.py
 *
 * G K W A R T I E N S 
 * V I J F U J V O O R 
 * M O V E R A H A L F 
 * X M P V T N J N T R 
 * Z A U V I J F N W Z 
 * E R C J E E D E A E 
 * S L K H N L R G A V 
 * J O F C T L I E L E 
 * L E R T W E E N F N 
 * D S T R P G E U U R 
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
  0,      4,       4,       4,     4,     65540,  65536, 65536,  65536,  0,
  2048,   2048,    2048,    2048,  0,     524288, 8,     8,      8,      8,
  0,      16,      16,      16,    16,    524288, 32,    32,     32,     32,
  0,      1048576, 2097152, 1024,  0,     524288, 0,     0,      64,     0,
  4096,   1064960, 2097152, 0,     1024,  524288, 0,     32768,  64,     8192,
  135168, 1048576, 2113536, 0,     0,     525312, 512,   32768,  64,     8192,
  4096,   1179648, 2097152, 16384, 0,     524288, 1536,  32768,  64,     8192,
  0,      1048576, 131072,  0,     16384, 524288, 512,   32768,  64,     8192,
  0,      1048576, 0,       256,   256,   524672, 896,   32896,  64,     8192,
  0,      1048576, 0,       0,     0,     0,      0,     262144, 262144, 262144
};

