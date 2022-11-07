.phony: all clean install uninstall

CC = gcc
CFLAGS = -std=c99 -O3 -march=native -pipe -pedantic -Wall -s -D_FORTIFY_SOURCE=1
LDFLAGS = -lcrypt

all: xte

install: xte
	cp xte /usr/bin/xte
	chown root:root /usr/bin/xte
	chmod 755 /usr/bin/xte
	chmod u+s /usr/bin/xte

uninstall:
	rm -f /usr/bin/xte

xte: xte.c
	$(CC) $(CFLAGS) $(LDFLAGS) -DXID=$(shell id -u) -o $@ $^

clean:
	rm -f $(BIN)
