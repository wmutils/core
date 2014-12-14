include ../config.mk

SRC =           \
	pfw.1   \
	lsw.1   \
	mapw.1  \
	killw.1 \
	wattr.1 \
	wtp.1   \
	wmv.1   \
	chwso.1 \
	wtf.1   \
	wrs.1   \
	chwb.1  \
	ignw.1  \
	wname.1

MAN = $(SRC:.1=.1.gz)

.POSIX:
.SUFFIXES: .1 .1.gz

all: $(MAN)

.1.1.gz:
	@echo "GZ $<"
	@$(GZ) -c $< > $@

install: $(MAN)
	mkdir -p $(DESTDIR)$(MANPREFIX)/man1/
	cp -f $(MAN) $(DESTDIR)$(MANPREFIX)/man1/

uninstall:
	@echo "uninstalling manpages"
	@for page in $(MAN); do \
		rm -f $(DESTDIR)$(MANPREFIX)/man1/$$page; \
	done

clean :
	rm -f $(MAN)
