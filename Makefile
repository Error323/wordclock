CC=clang
CFLAGS= -O3 -Wall -Wextra -march=native
DBGFLAGS= -O2 -g -Wall -Wextra
LDFLAGS=

all:
	$(CC) -o wordclock $(CFLAGS) clock.c $(LDFLAGS)

dbg:
	$(CC) -o wordclock $(DBGFLAGS) clock.c $(LDFLAGS)
