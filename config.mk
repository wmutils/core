PREFIX = /usr
MANPREFIX = $(PREFIX)/man

CC      = cc
LD      = $(CC)

CFLAGS  = -std=c99 -pedantic -Wall -Os
LDFLAGS = -lxcb -lxcb-util

