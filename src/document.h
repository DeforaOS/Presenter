/* $Id$ */
/* Copyright (c) 2014 Pierre Pronchery <khorben@defora.org> */
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


#ifndef PRESENTER_DOCUMENT_H
# define PRESENTER_DOCUMENT_H

# include "Presenter/presenter.h"
# include "slide.h"


/* public */
/* types */
typedef struct _PresenterDocument PresenterDocument;


/* functions */
/* essential */
PresenterDocument * presenterdocument_new(char const * filename);
void presenterdocument_delete(PresenterDocument * document);

/* accessors */
char const * presenterdocument_get_filename(PresenterDocument * document);

char const * presenterdocument_get_property(PresenterDocument * document,
		char const * property);
char const * presenterdocument_get_slide_property(PresenterDocument * document,
		unsigned int slide, char const * property);

/* useful */
int presenterdocument_save(PresenterDocument * document);
int presenterdocument_save_as(PresenterDocument * document,
		char const * filename);

#endif /* !PRESENTER_DOCUMENT_H */
