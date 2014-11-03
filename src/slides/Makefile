TARGETS	= $(OBJDIR)embed.so $(OBJDIR)title.so
OBJDIR	=
PREFIX	= /usr/local
DESTDIR	=
LIBDIR	= $(PREFIX)/lib
CC	= cc
CPPFLAGSF= -I ../../include
CPPFLAGS=
CFLAGSF	= -W `pkg-config --cflags libDesktop`
CFLAGS	= -Wall -g -O2 -pedantic -fPIC
LDFLAGSF= -W `pkg-config --libs libDesktop`
AR	= ar
RANLIB	= ranlib
CCSHARED= $(CC) -shared
RM	= rm -f
LN	= ln -f
MKDIR	= mkdir -m 0755 -p
INSTALL	= install


all: $(TARGETS)

embed_OBJS = $(OBJDIR)embed.o
embed_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
embed_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)embed.so: $(embed_OBJS)
	$(CCSHARED) -o $(OBJDIR)embed.so $(embed_OBJS) $(embed_LDFLAGS)

title_OBJS = $(OBJDIR)title.o
title_CFLAGS = $(CPPFLAGSF) $(CPPFLAGS) $(CFLAGSF) $(CFLAGS)
title_LDFLAGS = $(LDFLAGSF) $(LDFLAGS)

$(OBJDIR)title.so: $(title_OBJS)
	$(CCSHARED) -o $(OBJDIR)title.so $(title_OBJS) $(title_LDFLAGS)

$(OBJDIR)embed.o: embed.c
	$(CC) $(embed_CFLAGS) -o $(OBJDIR)embed.o -c embed.c

$(OBJDIR)title.o: title.c
	$(CC) $(title_CFLAGS) -o $(OBJDIR)title.o -c title.c

clean:
	$(RM) -- $(embed_OBJS) $(title_OBJS)

distclean: clean
	$(RM) -- $(TARGETS)

install: $(TARGETS)
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Presenter/slides
	$(INSTALL) -m 0644 $(OBJDIR)embed.so $(DESTDIR)$(LIBDIR)/Presenter/slides/embed.so
	$(MKDIR) $(DESTDIR)$(LIBDIR)/Presenter/slides
	$(INSTALL) -m 0644 $(OBJDIR)title.so $(DESTDIR)$(LIBDIR)/Presenter/slides/title.so

uninstall:
	$(RM) -- $(DESTDIR)$(LIBDIR)/Presenter/slides/embed.so
	$(RM) -- $(DESTDIR)$(LIBDIR)/Presenter/slides/title.so

.PHONY: all clean distclean install uninstall
