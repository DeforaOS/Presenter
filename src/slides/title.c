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



#include <System.h>
#include "Presenter/slide.h"


/* Title */
/* private */
/* types */
typedef struct _PresenterSlidePlugin
{
	PresenterSlideHelper * helper;
} Title;


/* prototypes */
/* plug-in */
static Title * _title_init(PresenterSlideHelper * helper);
static void _title_destroy(Title * title);

static GtkWidget * _title_get_widget(Title * title, PresenterSlide * slide);


/* public */
/* variables */
PresenterSlideDefinition slide =
{
	"Title",
	NULL,
	NULL,
	_title_init,
	_title_destroy,
	_title_get_widget
};


/* protected */
/* functions */
/* plug-in */
/* title_init */
static Title * _title_init(PresenterSlideHelper * helper)
{
	Title * title;

	if((title = object_new(sizeof(*title))) == NULL)
		return NULL;
	/* initialization */
	title->helper = helper;
	return title;
}


/* title_destroy */
static void _title_destroy(Title * title)
{
	/* FIXME implement the rest (if necessary) */
	object_delete(title);
}


/* title_get_widget */
static GtkWidget * _title_get_widget(Title * title, PresenterSlide * slide)
{
	GtkWidget * ret;
	GtkWidget * widget;

#if GTK_CHECK_VERSION(3, 0, 0)
	ret = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
#else
	ret = gtk_vbox_new(FALSE, 4);
#endif
	/* FIXME really implement */
	widget = gtk_label_new("<Insert title here>");
	gtk_widget_modify_fg(widget, GTK_STATE_NORMAL,
			&title->helper->foreground);
	gtk_box_pack_start(GTK_BOX(ret), widget, FALSE, TRUE, 0);
	gtk_widget_show_all(ret);
	return ret;
}
