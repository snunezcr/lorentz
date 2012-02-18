CC=gcc
CFLAGS=-O2 -I.
LDFLAGS=-lm

all: lorenz bin2text

lorenz: lorenz.c
	$(CC) $(CFLAGS) $(@).c -o $(@) $(LDFLAGS)

bin2text: bin2text.c
	$(CC) $(CFLAGS) $(@).c -o $(@) $(LDFLAGS)

clean:
	rm -f lorenz
	rm -f bin2text
