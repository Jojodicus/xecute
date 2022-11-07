.phony: all clean install uninstall

CC = gcc
CFLAGS = -std=c99 -O3 -march=native -pipe -pedantic -Wall
LDFLAGS = -lcrypt

BIN = xte

all: $(BIN)

$(BIN): $(BIN).o
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o $(BIN)
