PREFIX = /usr
MANPREFIX = $(PREFIX)/man

CC      = cc
LD      = $(CC)

CFLAGS  = -std=c99 -pedantic -Wall -Os -I/usr/X11R6/include
LDFLAGS = -lxcb -lxcb-util -lxcb-cursor -L/usr/X11R6/lib

