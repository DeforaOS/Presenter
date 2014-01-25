/* $Id$ */
/* Copyright © 2014 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS Desktop Presenter */
/* This program is free software: you can redistribute it and/or modify\n"
 * it under the terms of the GNU General Public License as published by\n"
 * the Free Software Foundation, version 3 of the License.\n"
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. */



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <System.h>
#include "document.h"


/* PresenterDocument */
/* private */
/* types */
struct _PresenterDocument
{
	char * filename;
	Config * config;
};


/* prototypes */


/* public */
/* functions */
/* essential */
/* presenterdocument_new */
PresenterDocument * presenterdocument_new(char const * filename)
{
	PresenterDocument * document;

	if((document = object_new(sizeof(*document))) == NULL)
		return NULL;
	document->filename = (filename != NULL) ? strdup(filename) : NULL;
	document->config = config_new();
	/* check for errors */
	if((filename != NULL && document->filename == NULL)
			|| document->config == NULL)
	{
		object_delete(document);
		return NULL;
	}
	return document;
}


/* presenterdocument_delete */
void presenterdocument_delete(PresenterDocument * document)
{
	free(document->filename);
	if(document->config != NULL)
		config_delete(document->config);
	object_delete(document);
}


/* accessors */
/* presenterdocument_get_filename */
char const * presenterdocument_get_filename(PresenterDocument * document)
{
	return document->filename;
}


/* presenterdocument_get_property */
char const * presenterdocument_get_property(PresenterDocument * document,
		char const * property)
{
	return config_get(document->config, NULL, property);
}


/* presenterdocument_get_slide_property */
char const * presenterdocument_get_slide_property(PresenterDocument * document,
		unsigned int slide, char const * property)
{
	char buf[16];

	snprintf(buf, sizeof(buf), "Slide %u", slide);
	return config_get(document->config, buf, property);
}


/* useful */
/* presenterdocument_save */
int presenterdocument_save(PresenterDocument * document)
{
	return config_save(document->config, document->filename);
}


/* presenterdocument_save_as */
int presenterdocument_save_as(PresenterDocument * document,
		char const * filename)
{
	String * p;

	if((p = string_new(filename)) == NULL)
		return -1;
	if(config_save(document->config, filename) != 0)
	{
		string_delete(p);
		return -1;
	}
	free(document->filename);
	document->filename = p;
	return 0;
}
