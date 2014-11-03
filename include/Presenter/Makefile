OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install
INCLUDEDIR= $(PREFIX)/include


all:

clean:

distclean: clean

install:
	$(MKDIR) $(DESTDIR)$(PREFIX)/include/Desktop/Presenter
	$(INSTALL) -m 0644 presenter.h $(DESTDIR)$(PREFIX)/include/Desktop/Presenter/presenter.h
	$(MKDIR) $(DESTDIR)$(PREFIX)/include/Desktop/Presenter
	$(INSTALL) -m 0644 slide.h $(DESTDIR)$(PREFIX)/include/Desktop/Presenter/slide.h

uninstall:
	$(RM) -- $(DESTDIR)$(PREFIX)/include/Desktop/Presenter/presenter.h
	$(RM) -- $(DESTDIR)$(PREFIX)/include/Desktop/Presenter/slide.h

.PHONY: all clean distclean install uninstall
