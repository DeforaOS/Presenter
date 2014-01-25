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



#ifndef DESKTOP_PRESENTER_SLIDE_H
# define DESKTOP_PRESENTER_SLIDE_H

# include <gtk/gtk.h>
# include "presenter.h"


/* PresenterSlide */
/* public */
/* types */
typedef struct _PresenterSlidePlugin PresenterSlidePlugin;

typedef struct _PresenterSlideHelper
{
	Presenter * presenter;
	GdkColor background;
	GdkColor foreground;
	int (*error)(Presenter * presenter, char const * message, int ret);
} PresenterSlideHelper;

typedef struct _PresenterSlideDefinition
{
	char const * name;
	char const * icon;
	char const * description;
	PresenterSlidePlugin * (*init)(PresenterSlideHelper * helper);
	void (*destroy)(PresenterSlidePlugin * plugin);
	GtkWidget * (*get_widget)(PresenterSlidePlugin * plugin,
			PresenterSlide * slide);
} PresenterSlideDefinition;

#endif /* !DESKTOP_PRESENTER_SLIDE_H */
