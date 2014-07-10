TARGETS	= presenter.1 presenter.html
PREFIX	= /usr/local
DESTDIR	=
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

presenter.1: presenter.xml
	./docbook.sh -P "$(PREFIX)" -- "presenter.1"

presenter.html: manual.css.xml presenter.css.xml presenter.xml
	./docbook.sh -P "$(PREFIX)" -- "presenter.html"

clean:
	$(RM) -- $(presenter.1_OBJS) $(presenter.html_OBJS)
	./docbook.sh -c -P "$(PREFIX)" -- "presenter.html"

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	./docbook.sh -P "$(DESTDIR)$(PREFIX)" -i -- "presenter.1"
	./docbook.sh -P "$(DESTDIR)$(PREFIX)" -i -- "presenter.html"

uninstall:
	./docbook.sh -P "$(DESTDIR)$(PREFIX)" -u -- "presenter.1"
	./docbook.sh -P "$(DESTDIR)$(PREFIX)" -u -- "presenter.html"

.PHONY: all clean distclean install uninstall
