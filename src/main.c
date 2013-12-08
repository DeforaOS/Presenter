/* $Id$ */
/* Copyright (c) 2013 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS Desktop Presenter */
/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. */



#include <unistd.h>
#include <stdio.h>
#include <locale.h>
#include <libintl.h>
#include <gtk/gtk.h>
#include "presenter.h"
#include "../config.h"
#define _(string) gettext(string)

/* constants */
#ifndef PROGNAME
# define PROGNAME	"presenter"
#endif
#ifndef PREFIX
# define PREFIX		"/usr/local"
#endif
#ifndef DATADIR
# define DATADIR	PREFIX "/share"
#endif
#ifndef LOCALEDIR
# define LOCALEDIR	DATADIR "/locale"
#endif


/* private */
/* prototypes */
static int _presenter(char const * filename);

static int _error(char const * message, int ret);
static int _usage(void);


/* functions */
/* presenter */
static int _presenter(char const * filename)
{
	Presenter * presenter;

	if((presenter = presenter_new()) == NULL)
		return -1;
	if(filename != NULL)
		presenter_open(presenter, filename);
	gtk_main();
	presenter_delete(presenter);
	return 0;
}


/* error */
static int _error(char const * message, int ret)
{
	fputs(PROGNAME ": ", stderr);
	perror(message);
	return ret;
}


/* usage */
static int _usage(void)
{
	fprintf(stderr, _("Usage: %s [filename]\n"), PROGNAME);
	return 1;
}


/* public */
/* functions */
/* main */
int main(int argc, char * argv[])
{
	int o;

	if(setlocale(LC_ALL, "") == NULL)
		_error("setlocale", 1);
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);
	gtk_init(&argc, &argv);
	while((o = getopt(argc, argv, "")) != -1)
		switch(o)
		{
			default:
				return _usage();
		}
	if(optind != argc && optind + 1 != argc)
		return _usage();
	if(argc - optind == 1)
		return (_presenter(argv[optind]) == 0) ? 0 : 2;
	return (_presenter(NULL) == 0) ? 0 : 2;
}
