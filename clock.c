#define _XOPEN_SOURCE

#include <time.h>
#include <stdio.h>
#include <string.h>


enum { VIJF=17, TIEN=18, UUR=12, VOOR=13, OVER=14, KWART=15, HALF=16 };

static const char *words[] = { "TWAALF", "EEN", "TWEE",  "DRIE", "VIER",
                               "VIJF",   "ZES", "ZEVEN", "ACHT", "NEGEN",
                               "TIEN",   "ELF", "UUR",   "VOOR", "OVER",
                               "KWART",  "HALF", "VIJF", "TIEN" };

static const int wall[] = { 'M',  17,  'A', 18,  KWART, 'R', VOOR, OVER, 'L',
                            HALF, 'O', 3,   'E', 'S',   8,   'L',  6,    0,
                            11,   'O', 'V', 1,   'E',   'V', 2,    7,    'R',
                            'M',  4,   9,   5,   'I',   10,  'J',  'A',  'N',
                            UUR,  'J', 'E', 'L', 'L',   'E' };

int contains(const int list[], const int size, const int x)
{
  int i;
  for (i = 0; i < size; i++)
    if (list[i] == x)
      return 1;
  return 0;
}

int time2words(const struct tm *time, int active[])
{
  int h = time->tm_hour % 12;
  int m = (time->tm_min + 2) / 5;
  int i = 0;

  switch (m)
  {
    case 1: case 7:  active[i++] = VIJF;  active[i++] = OVER; break;
    case 2: case 8:  active[i++] = TIEN;  active[i++] = OVER; break;
    case 3:          active[i++] = KWART; active[i++] = OVER; break;
    case 4: case 10: active[i++] = TIEN;  active[i++] = VOOR; break;
    case 5: case 11: active[i++] = VIJF;  active[i++] = VOOR; break;
    case 9:          active[i++] = KWART; active[i++] = VOOR; break;
    default: break;
  }

  if (m < 4)
    active[i++] = h;
  else
  if (m < 9)
  {
    active[i++] = HALF;
    active[i++] = (h+1)%12;
  }
  else
    active[i++] = (h+1)%12;

  if (m == 0 || m == 12)
    active[i++] = UUR;

  return i;
}

void printwall(const int active[], const int n)
{
  unsigned int i, j, str_len, line_len = 0;
  for (i = 0; i < sizeof(wall)/sizeof(int); i++)
  {
    if (wall[i] < 65 && contains(active, n, wall[i]))
      printf("\033[m");
    else
      printf("\033[30m");

    if (wall[i] < 65)
    {
      str_len = strlen(words[wall[i]]);
      line_len += str_len;
      if (line_len > 10)
      {
        printf("\n");
        line_len = str_len;
      }

      for (j = 0; j < str_len; j++)
        printf("%c ", (char) words[wall[i]][j]);
    }
    else
    {
      line_len++;
      if (line_len > 10)
      {
        printf("\n");
        line_len = 1;
      }
      printf("%c ", (char)wall[i]);
    }
  }
  printf("\033[m\n");
}

int main(int argc, char *argv[])
{
  int active[5];
  int n = -1;

  if (argc == 2)
  {
    struct tm t;
    strptime(argv[1], "%H:%M", &t);
    n = time2words(&t, active);
  }
  else
  {
    struct tm *t;
    time_t cur_time = time(NULL);
    t = localtime(&cur_time);
    n = time2words(t, active);
  }

  printwall(active, n);

  return 0;
}
