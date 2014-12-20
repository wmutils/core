PREFIX:=/usr
MANPREFIX:=$(PREFIX)/share/man

CC      := cc
LD      := $(CC)
GZ      := gzip

CFLAGS  += -std=c99 -pedantic -Wall -Os
LDFLAGS += -lxcb

