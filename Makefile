CC=gcc
CFLAGS=-O2 -I.
LDFLAGS=-lm

all: lorenz exp2text

lorenz: lorenz.c
	$(CC) $(CFLAGS) $(@).c -o $(@) $(LDFLAGS)

exp2text: exp2text.c
	$(CC) $(CFLAGS) $(@).c -o $(@) $(LDFLAGS)

clean:
	rm -f lorenz
	rm -f exp2text
