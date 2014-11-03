TARGETS	= $(OBJDIR)presenter.1 $(OBJDIR)presenter.html
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

$(OBJDIR)presenter.1: presenter.xml
	./docbook.sh -P "$(PREFIX)" -- "$(OBJDIR)presenter.1"

$(OBJDIR)presenter.html: manual.css.xml presenter.css.xml presenter.xml
	./docbook.sh -P "$(PREFIX)" -- "$(OBJDIR)presenter.html"

clean:
	$(RM) -- $(presenter.1_OBJS) $(presenter.html_OBJS)
	./docbook.sh -c -P "$(PREFIX)" -- "presenter.1"
	./docbook.sh -c -P "$(PREFIX)" -- "presenter.html"

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	./docbook.sh -P "$(DESTDIR)$(PREFIX)" -i -- "$(OBJDIR)presenter.1"
	./docbook.sh -P "$(DESTDIR)$(PREFIX)" -i -- "$(OBJDIR)presenter.html"

uninstall:
	./docbook.sh -P "$(DESTDIR)$(PREFIX)" -u -- "presenter.1"
	./docbook.sh -P "$(DESTDIR)$(PREFIX)" -u -- "presenter.html"

.PHONY: all clean distclean install uninstall
