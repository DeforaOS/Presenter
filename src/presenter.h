/* $Id$ */
/* Copyright (c) 2013-2014 Pierre Pronchery <khorben@defora.org> */
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


#ifndef PRESENTER_PRESENTER_H
# define PRESENTER_PRESENTER_H

# include "Presenter.h"


/* public */
/* functions */
/* essential */
Presenter * presenter_new(void);
void presenter_delete(Presenter * presenter);

/* accessors */
int presenter_set_monitor(Presenter * presenter, int monitor);

/* useful */
int presenter_confirm(Presenter * presenter, char const * message, ...);
int presenter_error(Presenter * presenter, char const * message, int ret);

int presenter_close(Presenter * presenter);
int presenter_open(Presenter * presenter, char const * filename);
int presenter_open_dialog(Presenter * presenter);
int presenter_save(Presenter * presenter);
int presenter_save_as(Presenter * presenter, char const * filename);
int presenter_save_as_dialog(Presenter * presenter);

/* slide plug-ins */
int presenter_slide_load(Presenter * presenter, char const * type);
int presenter_slide_load_all(Presenter * presenter);

#endif /* !PRESENTER_PRESENTER_H */
