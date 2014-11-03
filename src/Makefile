SUBDIRS	= slides
TARGETS	= $(OBJDIR)presenter
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
BINDIR	= $(PREFIX)/bin
SBINDIR	= $(PREFIX)/sbin
CC	= cc
CPPFLAGSF= -I ../include
CPPFLAGS=
CFLAGSF	= -W `pkg-config --cflags libDesktop`
CFLAGS	= -Wall -g -O2
LDFLAGSF= `pkg-config --libs libDesktop`
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: subdirs $(TARGETS)

subdirs:
	@for i in $(SUBDIRS); do (cd "$$i" && $(MAKE)) || exit; done

presenter_OBJS = $(OBJDIR)document.o $(OBJDIR)presenter.o $(OBJDIR)slide.o $(OBJDIR)main.o
presenter_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
presenter_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)presenter: $(presenter_OBJS)
	$(CC) -o $(OBJDIR)presenter $(presenter_OBJS) $(presenter_LDFLAGS)

$(OBJDIR)document.o: document.c document.h slide.h ../config.h
	$(CC) $(presenter_CFLAGS) -o $(OBJDIR)document.o -c document.c

$(OBJDIR)presenter.o: presenter.c document.h presenter.h slide.h ../config.h
	$(CC) -D PREFIX=\"$(PREFIX)\" $(presenter_CFLAGS) -o $(OBJDIR)presenter.o -c presenter.c

$(OBJDIR)slide.o: slide.c slide.h ../config.h
	$(CC) $(presenter_CFLAGS) -o $(OBJDIR)slide.o -c slide.c

$(OBJDIR)main.o: main.c presenter.h ../config.h
	$(CC) $(presenter_CFLAGS) -o $(OBJDIR)main.o -c main.c

clean:
	@for i in $(SUBDIRS); do (cd "$$i" && $(MAKE) clean) || exit; done
	$(RM) -- $(presenter_OBJS)

distclean:
	@for i in $(SUBDIRS); do (cd "$$i" && $(MAKE) distclean) || exit; done
	$(RM) -- $(presenter_OBJS)
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	@for i in $(SUBDIRS); do (cd "$$i" && $(MAKE) install) || exit; done
	$(MKDIR) $(DESTDIR)$(BINDIR)
	$(INSTALL) -m 0755 $(OBJDIR)presenter $(DESTDIR)$(BINDIR)/presenter

uninstall:
	@for i in $(SUBDIRS); do (cd "$$i" && $(MAKE) uninstall) || exit; done
	$(RM) -- $(DESTDIR)$(BINDIR)/presenter

.PHONY: all subdirs clean distclean install uninstall
