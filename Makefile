include config.mk

HDR = arg.h util.h
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
	wname.c \
	wmp.c

OBJ = $(SRC:.c=.o)
BIN = $(SRC:.c=)
MAN = $(SRC:.c=.1.gz)

.POSIX:
.SUFFIXES: .1 .1.gz

all: binutils manpages

binutils: $(BIN)

manpages:
	cd man; $(MAKE) $(MFLAGS)

$(OBJ): $(HDR) util.o

.o:
	@echo "LD $@"
	@$(LD) $< -o $@ $(LDFLAGS) util.o

.c.o:
	@echo "CC $<"
	@$(CC) -c $< -o $@ $(CFLAGS)

install: $(BIN)
	mkdir -p $(DESTDIR)$(PREFIX)/bin/
	cp -f $(BIN) $(DESTDIR)$(PREFIX)/bin/
	cd man; $(MAKE) $(MFLAGS) install

uninstall:
	@echo "uninstalling binaries"
	@for util in $(BIN); do \
		rm -f $(DESTDIR)$(PREFIX)/bin/$$util; \
	done
	cd man; $(MAKE) $(MFLAGS) uninstall

clean :
	rm -f $(OBJ) $(BIN) util.o
	cd man; $(MAKE) $(MFLAGS) clean
