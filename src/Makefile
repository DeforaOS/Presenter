TARGETS	= presenter
PREFIX	= /usr/local
DESTDIR	=
BINDIR	= $(PREFIX)/bin
SBINDIR	= $(PREFIX)/sbin
CC	= cc
CPPFLAGSF=
CPPFLAGS=
CFLAGSF	= -W `pkg-config --cflags libDesktop`
CFLAGS	= -Wall -g -O2
LDFLAGSF= `pkg-config --libs libDesktop`
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

presenter_OBJS = presenter.o main.o
presenter_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
presenter_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

presenter: $(presenter_OBJS)
	$(CC) -o presenter $(presenter_OBJS) $(presenter_LDFLAGS)

presenter.o: presenter.c presenter.h ../config.h
	$(CC) -D PREFIX=\"$(PREFIX)\" $(presenter_CFLAGS) -c presenter.c

main.o: main.c presenter.h ../config.h
	$(CC) $(presenter_CFLAGS) -c main.c

clean:
	$(RM) -- $(presenter_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	$(MKDIR) $(DESTDIR)$(BINDIR)
	$(INSTALL) -m 0755 presenter $(DESTDIR)$(BINDIR)/presenter

uninstall:
	$(RM) -- $(DESTDIR)$(BINDIR)/presenter

.PHONY: all clean distclean install uninstall
