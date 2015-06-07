/* Settings */
#define LDR_PIN  0 // Light sensor pin
#define LED_PIN  3 // Matrix led pin
#define DBG_PIN  7 // Board debug led pin
#define DCF_PIN  2 // Radio clock pin
#define SIZE    10 // Matrix board size x size
#define FPS     40 // Frames per second to achieve
#define ANI_TME 10 // Animation time in seconds

/* Globals */
/** @brief color */
struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

/** @brief earthtone colors
 * http://www.creativecolorschemes.com/resources/free-color-schemes/earth-tone-color-scheme.shtml
 */
static Color colors[] = { { 73, 58, 41 },
                          { 120, 109, 91 },
                          { 169, 161, 140 },
                          { 97, 51, 24 },
                          { 133, 87, 35 },
                          { 185, 156, 107 },
                          { 143, 59, 27 },
                          { 213, 117, 0 },
                          { 219, 202, 105 },
                          { 64, 79, 36 },
                          { 102, 141, 60 },
                          { 189, 208, 156 },
                          { 78, 97, 114 },
                          { 131, 146, 159 },
                          { 163, 172, 184 } };
