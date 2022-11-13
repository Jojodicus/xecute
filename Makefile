############
# SETTINGS #
############

# id of privileged user
XID = $(shell id -u)
# time in seconds on how long a persistent session should last
# 0:	never ask for password
# <0:	always ask for password
SESSION_TIME = 300
# path where the session file should be stored
SESSION_FILE = \"/run/xte.sess\"
# how many tries the user has
MAXTRIES = 3
# how many seconds the user is timeouted if not successfully authenticated
# 0: disabled
TIMEOUT = 60

############

CC = gcc
CFLAGS = -std=c99 -pipe -pedantic -Wall
DFLAGS = -O0 -g
RFLAGS = -O3 -march=native -s -D_FORTIFY_SOURCE=1
LDFLAGS = -lcrypt
DEFINES = -DXID=$(XID) -DSESSION_TIME=$(SESSION_TIME) -DSESSION_FILE=$(SESSION_FILE) -DMAXTRIES=$(MAXTRIES) -DTIMEOUT=$(TIMEOUT)

all: xte.c
	$(CC) $(CFLAGS) $(RFLAGS) $(LDFLAGS) $(DEFINES) -o xte $^

debug: xte.c
	$(CC) $(CFLAGS) $(DFLAGS) $(LDFLAGS) $(DEFINES) -o xte $^

install: xte
	install -m 755 -o root -g root xte /usr/bin/xte
	chmod u+s /usr/bin/xte

uninstall:
	rm -f /usr/bin/xte

clean:
	rm -f xte

# for compatability
xte: xte.c
	$(CC) $(CFLAGS) $(RFLAGS) $(LDFLAGS) $(DEFINES) -o $@ $^

.phony: all clean install uninstall
