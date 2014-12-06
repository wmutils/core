PREFIX:=/usr
MANPREFIX:=$(PREFIX)/share/man

CC      := cc
LD      := $(CC)
CFLAGS  += -std=c99 -pedantic -Wall -Os
LDFLAGS += -lxcb

HDR = arg.h
SRC =           \
	pfw.c   \
	lsw.c   \
	mapw.c  \
	killw.c \
	wattr.c \
	wtp.c   \
	wmv.c   \
	chwso.c \
	wtf.c   \
	wrs.c   \
	chwb.c  \
	ignw.c  \
	wname.c

OBJ = $(SRC:.c=.o)
BIN = $(SRC:.c=)

.POSIX:

all: $(BIN)

$(OBJ): $(HDR)

.o:
	@echo "LD $@"
	@$(LD) $< -o $@ $(LDFLAGS)

.c.o:
	@echo "CC $<"
	@$(CC) -c $< -o $@ $(CFLAGS)

install: $(BIN)
	mkdir -p $(DESTDIR)$(PREFIX)/bin/
	cp -f $(BIN) $(DESTDIR)$(PREFIX)/bin/

uninstall:
	@echo "uninstalling $(BIN)"
	for util in $(BIN); do \
		rm -f $(DESTDIR)$(PREFIX)/bin/$$util; \
	done

clean :
	rm -f $(OBJ) $(BIN)
