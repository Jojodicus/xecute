############
# SETTINGS #
############
# change settings that depend on shell-commands

# id of privileged user
XID = $(shell id -u)
# number of digits for the maximum allowed process id
PID_MAX_DIGITS = $(shell cat /proc/sys/kernel/pid_max | wc -m)

DEFINES = -DXID=$(XID) -DPID_MAX_DIGITS=$(PID_MAX_DIGITS) # add options here if desired

############

CC = gcc
CFLAGS = -std=c99 -pipe -pedantic -Wall -Werror=vla
DFLAGS = -O0 -g
RFLAGS = -O3 -march=native -s -D_FORTIFY_SOURCE=1
LDFLAGS = -lcrypt

all: xte

debug: xte.c config.h
	$(CC) $(CFLAGS) $(DFLAGS) $(LDFLAGS) $(DEFINES) -o xte $<

install: xte
	install -Dm 4755 -o root -g root xte $(DESTDIR)/usr/bin/xte
	ln -sf $(DESTDIR)/usr/bin/xte $(DESTDIR)/usr/bin/xecute

uninstall:
	rm -f $(DESTDIR)/usr/bin/xte $(DESTDIR)/usr/bin/xecute

clean:
	rm -f xte

xte: xte.c config.h
	$(CC) $(CFLAGS) $(RFLAGS) $(DEFINES) -o $@ $< $(LDFLAGS)

.phony: all clean install uninstall
