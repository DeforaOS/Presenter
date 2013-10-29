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


#ifndef PRESENTER_PRESENTER_H
# define PRESENTER_PRESENTER_H


/* public */
/* types */
typedef struct _Presenter Presenter;


/* functions */
/* essential */
Presenter * presenter_new(void);
void presenter_delete(Presenter * presenter);

/* useful */
int presenter_error(Presenter * presenter, char const * message, int ret);

int presenter_close(Presenter * presenter);
int presenter_open(Presenter * presenter, char const * filename);
int presenter_open_dialog(Presenter * presenter);

#endif /* !PRESENTER_PRESENTER_H */
