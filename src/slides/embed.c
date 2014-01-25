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
#if GTK_CHECK_VERSION(3, 0, 0)
# include <gtk/gtkx.h>
#endif


/* Embed */
/* private */
/* types */
typedef struct _PresenterSlidePlugin
{
	PresenterSlideHelper * helper;
} Embed;


/* prototypes */
/* plug-in */
static Embed * _embed_init(PresenterSlideHelper * helper);
static void _embed_destroy(Embed * embed);

static GtkWidget * _embed_get_widget(Embed * embed, PresenterSlide * slide);


/* public */
/* variables */
PresenterSlideDefinition slide =
{
	"Embed",
	NULL,
	NULL,
	_embed_init,
	_embed_destroy,
	_embed_get_widget
};


/* protected */
/* functions */
/* plug-in */
/* embed_init */
static Embed * _embed_init(PresenterSlideHelper * helper)
{
	Embed * embed;

	if((embed = object_new(sizeof(*embed))) == NULL)
		return NULL;
	/* initialization */
	embed->helper = helper;
	return embed;
}


/* embed_destroy */
static void _embed_destroy(Embed * embed)
{
	/* FIXME implement the rest */
	object_delete(embed);
}


/* embed_get_widget */
static GtkWidget * _embed_get_widget(Embed * embed, PresenterSlide * slide)
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
			&embed->helper->foreground);
	gtk_box_pack_start(GTK_BOX(ret), widget, FALSE, TRUE, 0);
	widget = gtk_socket_new();
	gtk_widget_modify_bg(widget, GTK_STATE_NORMAL,
			&embed->helper->background);
	gtk_box_pack_start(GTK_BOX(ret), widget, TRUE, TRUE, 0);
	gtk_widget_show_all(ret);
	return ret;
}
