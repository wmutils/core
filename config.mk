PREFIX = /usr/local
MANPREFIX = $(PREFIX)/man

CC      = cc
LD      = $(CC)

CFLAGS  = -std=c99 -pedantic -Wall -Os
LDFLAGS = -lxcb -lwm

