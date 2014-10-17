CC=clang
CFLAGS= -O3 -Wall -Wextra -march=native
DBGFLAGS= -O2 -g -Wall -Wextra
LDFLAGS=

all:
	$(CC) -o wordclock.exe $(CFLAGS) clock.c $(LDFLAGS)

dbg:
	$(CC) -o wordclock.exe $(DBGFLAGS) clock.c $(LDFLAGS)
