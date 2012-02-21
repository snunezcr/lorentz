CC=gcc
CFLAGS=-O2 -I.
LDFLAGS=-lm

all: lorenz slorenz exp2text statistics stat2text

lorenz: lorenz.c
	$(CC) $(CFLAGS) $(@).c -o $(@) $(LDFLAGS)

slorenz: slorenz.c
	$(CC) $(CFLAGS) $(@).c ranlib.c linpack.c com.c -o $(@) $(LDFLAGS)

exp2text: exp2text.c
	$(CC) $(CFLAGS) $(@).c -o $(@) $(LDFLAGS)

statistics: statistics.c
	$(CC) $(CFLAGS) $(@).c -o $(@) $(LDFLAGS)

stat2text: stat2text.c
	$(CC) $(CFLAGS) $(@).c -o $(@) $(LDFLAGS)

clean:
	rm -f lorenz
	rm -f slorenz
	rm -f exp2text
	rm -f statistics
	rm -f stat2text
