include config.mk

HDR = arg.h util.h
SRC =           \
	pfw.c   \
	pdw.c   \
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
	wmp.c   \
	slw.c   \
	atomx.c

OBJ = $(SRC:.c=.o)
BIN = $(SRC:.c=)
MAN = $(SRC:.c=.1)

.POSIX:
.SUFFIXES: .1 .1.gz

all: binutils

binutils: $(BIN)

$(OBJ): $(HDR) util.o

.o:
	@echo "LD $@"
	@$(LD) $< util.o -o $@ $(LDFLAGS)

.c.o:
	@echo "CC $<"
	@$(CC) -c $< -o $@ $(CFLAGS)

install: $(BIN)
	mkdir -p $(DESTDIR)$(PREFIX)/bin/
	cp -f $(BIN) $(DESTDIR)$(PREFIX)/bin/
	cd man; $(MAKE) install

uninstall:
	@echo "uninstalling binaries"
	@for util in $(BIN); do \
		rm -f $(DESTDIR)$(PREFIX)/bin/$$util; \
	done
	cd man; $(MAKE) uninstall

clean :
	rm -f $(OBJ) $(BIN) util.o
