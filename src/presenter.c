/* $Id$ */
static char const _copyright[] =
"Copyright © 2013 Pierre Pronchery <khorben@defora.org>";
/* This file is part of DeforaOS Desktop Presenter */
static char const _license[] =
"This program is free software: you can redistribute it and/or modify\n"
"it under the terms of the GNU General Public License as published by\n"
"the Free Software Foundation, version 3 of the License.\n"
"\n"
"This program is distributed in the hope that it will be useful,\n"
"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
"GNU General Public License for more details.\n"
"\n"
"You should have received a copy of the GNU General Public License\n"
"along with this program.  If not, see <http://www.gnu.org/licenses/>.\n";


#include <stdio.h>
#include <libintl.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <System.h>
#include <Desktop.h>
#include "presenter.h"
#include "../config.h"
#define _(string) gettext(string)
#define N_(string) (string)


/* Presenter */
/* private */
/* types */
struct _Presenter
{
	char * filename;

	Config * config;

	/* widgets */
	GtkWidget * window;
	GtkWidget * view;
	GtkWidget * statusbar;
#if GTK_CHECK_VERSION(2, 18, 0)
	/* infobar */
	GtkWidget * infobar;
	GtkWidget * infobar_label;
#endif
	/* preferences */
	GtkWidget * pr_window;
	/* find */
	GtkWidget * fi_dialog;
	GtkListStore * fi_store;
	GtkWidget * fi_text;
	/* about */
	GtkWidget * ab_window;
};


/* prototypes */
/* callbacks */
#ifndef EMBEDDED
static void _presenter_on_about(gpointer data);
#endif
static void _presenter_on_close(gpointer data);
static gboolean _presenter_on_closex(gpointer data);
static void _presenter_on_contents(gpointer data);
static void _presenter_on_copy(gpointer data);
static void _presenter_on_cut(gpointer data);
static void _presenter_on_find(gpointer data);
static void _presenter_on_insert_file(gpointer data);
static void _presenter_on_new(gpointer data);
static void _presenter_on_open(gpointer data);
static void _presenter_on_paste(gpointer data);
static void _presenter_on_preferences(gpointer data);
static void _presenter_on_properties(gpointer data);
static void _presenter_on_save(gpointer data);
static void _presenter_on_save_as(gpointer data);
static void _presenter_on_select_all(gpointer data);
static void _presenter_on_slideshow(gpointer data);
#ifndef EMBEDDED
static void _presenter_on_unselect_all(gpointer data);
#endif


/* variables */
static char const * _authors[] =
{
	"Pierre Pronchery <khorben@defora.org>",
	NULL
};

#ifdef EMBEDDED
static const DesktopAccel _presenter_accel[] =
{
	{ G_CALLBACK(_presenter_on_close), GDK_CONTROL_MASK, GDK_KEY_W },
	{ G_CALLBACK(_presenter_on_contents), 0, GDK_KEY_F1 },
	{ G_CALLBACK(_presenter_on_new), GDK_CONTROL_MASK, GDK_KEY_N },
	{ G_CALLBACK(_presenter_on_open), GDK_CONTROL_MASK, GDK_KEY_O },
	{ G_CALLBACK(_presenter_on_save), GDK_CONTROL_MASK, GDK_KEY_S },
	{ G_CALLBACK(_presenter_on_save_as), GDK_CONTROL_MASK | GDK_SHIFT_MASK,
		GDK_KEY_S },
	{ NULL, 0, 0 }
};
#endif

#ifndef EMBEDDED
static const DesktopMenu _presenter_menu_file[] =
{
	{ N_("_New"), G_CALLBACK(_presenter_on_new), GTK_STOCK_NEW,
		GDK_CONTROL_MASK, GDK_KEY_N },
	{ N_("_Open"), G_CALLBACK(_presenter_on_open), GTK_STOCK_OPEN,
		GDK_CONTROL_MASK, GDK_KEY_O },
	{ "", NULL, NULL, 0, 0 },
	{ N_("_Save"), G_CALLBACK(_presenter_on_save), GTK_STOCK_SAVE,
		GDK_CONTROL_MASK, GDK_KEY_S },
	{ N_("_Save as..."), G_CALLBACK(_presenter_on_save_as),
		GTK_STOCK_SAVE_AS, GDK_CONTROL_MASK | GDK_SHIFT_MASK,
		GDK_KEY_S },
	{ "", NULL, NULL, 0, 0 },
	{ N_("_Properties"), G_CALLBACK(_presenter_on_properties),
		GTK_STOCK_PROPERTIES, GDK_MOD1_MASK, GDK_KEY_Return },
	{ "", NULL, NULL, 0, 0 },
	{ N_("_Close"), G_CALLBACK(_presenter_on_close), GTK_STOCK_CLOSE, 0,
		0 },
	{ NULL, NULL, NULL, 0, 0 }
};

static const DesktopMenu _presenter_menu_edit[] =
{
	/* FIXME implement undo and redo */
	{ N_("_Undo"), NULL, GTK_STOCK_UNDO, GDK_CONTROL_MASK,
		GDK_KEY_Z },
	{ N_("_Redo"), NULL, GTK_STOCK_REDO, GDK_CONTROL_MASK, GDK_KEY_R },
	{ "", NULL, NULL, 0, 0 },
	{ N_("Cu_t"), G_CALLBACK(_presenter_on_cut), GTK_STOCK_CUT,
		GDK_CONTROL_MASK, GDK_KEY_X },
	{ N_("_Copy"), G_CALLBACK(_presenter_on_copy), GTK_STOCK_COPY,
		GDK_CONTROL_MASK, GDK_KEY_C },
	{ N_("_Paste"), G_CALLBACK(_presenter_on_paste), GTK_STOCK_PASTE,
		GDK_CONTROL_MASK, GDK_KEY_V },
	{ "", NULL, NULL, 0, 0 },
	{ N_("Select _all"), G_CALLBACK(_presenter_on_select_all),
#if GTK_CHECK_VERSION(2, 10, 0)
		GTK_STOCK_SELECT_ALL,
#else
		"edit-select-all",
#endif
		GDK_CONTROL_MASK, GDK_KEY_A },
	{ N_("_Unselect all"), G_CALLBACK(_presenter_on_unselect_all), NULL, 0,
		0 },
	{ "", NULL, NULL, 0, 0 },
	{ N_("_Find"), G_CALLBACK(_presenter_on_find), GTK_STOCK_FIND,
		GDK_CONTROL_MASK, GDK_KEY_F },
	{ "", NULL, NULL, 0, 0 },
	{ N_("_Preferences"), G_CALLBACK(_presenter_on_preferences),
		GTK_STOCK_PREFERENCES, GDK_CONTROL_MASK, GDK_KEY_P },
	{ NULL, NULL, NULL, 0, 0 }
};

static const DesktopMenu _presenter_menu_insert[] =
{
	{ N_("_File..."), G_CALLBACK(_presenter_on_insert_file), "insert-text",
		0, 0 },
	{ NULL, NULL, NULL, 0, 0 }
};

static const DesktopMenu _presenter_menu_view[] =
{
	{ N_("_Slideshow"), G_CALLBACK(_presenter_on_slideshow),
		"x-office-presentation", 0, GDK_KEY_F5 },
	{ NULL, NULL, NULL, 0, 0 }
};

static const DesktopMenu _presenter_menu_help[] =
{
        { N_("_Contents"), G_CALLBACK(_presenter_on_contents), "help-contents",
		0, GDK_KEY_F1 },
        { N_("_About"), G_CALLBACK(_presenter_on_about),
#if GTK_CHECK_VERSION(2, 6, 0)
                GTK_STOCK_ABOUT, 0, 0 },
#else
                NULL, 0, 0 },
#endif
        { NULL, NULL, NULL, 0, 0 }
};

static const DesktopMenubar _presenter_menubar[] =
{
	{ N_("_File"), _presenter_menu_file },
	{ N_("_Edit"), _presenter_menu_edit },
	{ N_("_Insert"), _presenter_menu_insert },
	{ N_("_View"), _presenter_menu_view },
	{ N_("_Help"), _presenter_menu_help },
	{ NULL, NULL }
};
#endif

static DesktopToolbar _presenter_toolbar[] =
{
	{ N_("New"), G_CALLBACK(_presenter_on_new), GTK_STOCK_NEW, 0, 0, NULL },
	{ N_("Open"), G_CALLBACK(_presenter_on_open), GTK_STOCK_OPEN, 0, 0,
		NULL },
	{ "", NULL, NULL, 0, 0, NULL },
	{ N_("Save"), G_CALLBACK(_presenter_on_save), GTK_STOCK_SAVE, 0, 0,
		NULL },
	{ N_("Save as"), G_CALLBACK(_presenter_on_save_as), GTK_STOCK_SAVE_AS,
		0, 0, NULL },
	{ "", NULL, NULL, 0, 0, NULL },
	{ N_("Cut"), G_CALLBACK(_presenter_on_cut), GTK_STOCK_CUT, 0, 0, NULL },
	{ N_("Copy"), G_CALLBACK(_presenter_on_copy), GTK_STOCK_COPY, 0, 0,
		NULL },
	{ N_("Paste"), G_CALLBACK(_presenter_on_paste), GTK_STOCK_PASTE, 0, 0,
		NULL },
#ifdef EMBEDDED
	{ "", NULL, NULL, 0, 0, NULL },
	{ N_("Find"), G_CALLBACK(_presenter_on_find), GTK_STOCK_FIND,
		GDK_CONTROL_MASK, GDK_KEY_F, NULL },
	{ N_("Select all"), G_CALLBACK(_presenter_on_select_all),
		GTK_STOCK_SELECT_ALL, GDK_CONTROL_MASK, GDK_KEY_A, NULL },
	{ "", NULL, NULL, 0, 0, NULL },
	{ N_("Preferences"), G_CALLBACK(_presenter_on_preferences),
		GTK_STOCK_PREFERENCES, GDK_CONTROL_MASK, GDK_KEY_P, NULL },
	{ N_("Properties"), G_CALLBACK(_presenter_on_properties),
		GTK_STOCK_PROPERTIES, GDK_MOD1_MASK, GDK_KEY_Return, NULL },
	{ "", NULL, NULL, 0, 0, NULL },
	{ N_("Insert file"), G_CALLBACK(_presenter_on_insert_file),
		"insert-text", 0, 0, NULL },
#endif
	{ "", NULL, NULL, 0, 0, NULL },
	{ N_("Slideshow"), G_CALLBACK(_presenter_on_slideshow),
		"x-office-presentation", 0, GDK_KEY_F5, NULL },
#ifdef EMBEDDED
	{ "", NULL, NULL, 0, 0, NULL },
	{ N_("Help"), G_CALLBACK(_presenter_on_contents), "help-contents",
		0, GDK_KEY_F1, NULL },
#endif
	{ NULL, NULL, NULL, 0, 0, NULL }
};


/* public */
/* functions */
/* essential */
/* presenter_new */
static void _new_set_title(Presenter * presenter);

Presenter * presenter_new(void)
{
	Presenter * presenter;
	GtkAccelGroup * group;
	GtkWidget * vbox;
	GtkWidget * widget;

	if((presenter = object_new(sizeof(*presenter))) == NULL)
		return NULL;
	presenter->filename = NULL;
	presenter->config = config_new();
	/* check for errors */
	if(presenter->config == NULL)
	{
		object_delete(presenter);
		return NULL;
	}
	/* widgets */
	group = gtk_accel_group_new();
	/* window */
	presenter->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_add_accel_group(GTK_WINDOW(presenter->window), group);
	gtk_window_set_default_size(GTK_WINDOW(presenter->window), 640, 480);
	g_signal_connect_swapped(presenter->window, "delete-event",
			G_CALLBACK(_presenter_on_closex), presenter);
	_new_set_title(presenter);
#if GTK_CHECK_VERSION(2, 6, 0)
	gtk_window_set_icon_name(GTK_WINDOW(presenter->window),
			"x-office-presentation");
#endif
	vbox = gtk_vbox_new(FALSE, 0);
	/* menubar */
#ifndef EMBEDDED
	widget = desktop_menubar_create(_presenter_menubar, presenter, group);
	gtk_box_pack_start(GTK_BOX(vbox), widget, FALSE, TRUE, 0);
#else
	desktop_accel_create(_presenter_accel, presenter, group);
#endif
	/* toolbar */
	widget = desktop_toolbar_create(_presenter_toolbar, presenter, group);
	gtk_box_pack_start(GTK_BOX(vbox), widget, FALSE, TRUE, 0);
	g_object_unref(group);
#if GTK_CHECK_VERSION(2, 18, 0)
	/* infobar */
	presenter->infobar = gtk_info_bar_new_with_buttons(GTK_STOCK_CLOSE,
			GTK_RESPONSE_CLOSE, NULL);
	gtk_info_bar_set_message_type(GTK_INFO_BAR(presenter->infobar),
			GTK_MESSAGE_ERROR);
	g_signal_connect(presenter->infobar, "close",
			G_CALLBACK(gtk_widget_hide), NULL);
	g_signal_connect(presenter->infobar, "response",
			G_CALLBACK(gtk_widget_hide), NULL);
	widget = gtk_info_bar_get_content_area(
			GTK_INFO_BAR(presenter->infobar));
	presenter->infobar_label = gtk_label_new(NULL);
	gtk_widget_show(presenter->infobar_label);
	gtk_box_pack_start(GTK_BOX(widget), presenter->infobar_label, TRUE,
			TRUE, 0);
	gtk_widget_set_no_show_all(presenter->infobar, TRUE);
	gtk_box_pack_start(GTK_BOX(vbox), presenter->infobar, FALSE, TRUE, 0);
#endif
	/* view */
	/* FIXME implement */
	/* statusbar */
	presenter->statusbar = gtk_statusbar_new();
	gtk_box_pack_end(GTK_BOX(vbox), presenter->statusbar, FALSE, TRUE, 0);
	gtk_container_add(GTK_CONTAINER(presenter->window), vbox);
	gtk_widget_show_all(presenter->window);
	return presenter;
}

static void _new_set_title(Presenter * presenter)
{
	char buf[256];

	snprintf(buf, sizeof(buf), "%s%s", _("Presenter - "),
			(presenter->filename == NULL) ? _("(Untitled)")
			: presenter->filename);
	gtk_window_set_title(GTK_WINDOW(presenter->window), buf);
}


/* presenter_delete */
void presenter_delete(Presenter * presenter)
{
	object_delete(presenter);
}


/* useful */
/* presenter_about */
/* callbacks */
static gboolean _about_on_closex(gpointer data);

void presenter_about(Presenter * presenter)
{
	if(presenter->ab_window != NULL)
	{
		gtk_widget_show(presenter->ab_window);
		return;
	}
	presenter->ab_window = desktop_about_dialog_new();
	gtk_window_set_transient_for(GTK_WINDOW(presenter->ab_window),
			GTK_WINDOW(presenter->window));
	desktop_about_dialog_set_authors(presenter->ab_window, _authors);
	desktop_about_dialog_set_comments(presenter->ab_window,
			_("Presentation tool for the DeforaOS desktop"));
	desktop_about_dialog_set_copyright(presenter->ab_window, _copyright);
	desktop_about_dialog_set_logo_icon_name(presenter->ab_window,
			"x-office-presentation");
	desktop_about_dialog_set_license(presenter->ab_window, _license);
	desktop_about_dialog_set_name(presenter->ab_window, PACKAGE);
	desktop_about_dialog_set_translator_credits(presenter->ab_window,
			_("translator-credits"));
	desktop_about_dialog_set_version(presenter->ab_window, VERSION);
	desktop_about_dialog_set_website(presenter->ab_window,
			"http://www.defora.org/");
	g_signal_connect_swapped(G_OBJECT(presenter->ab_window), "delete-event",
			G_CALLBACK(_about_on_closex), presenter);
	gtk_widget_show(presenter->ab_window);
}

static gboolean _about_on_closex(gpointer data)
{
	Presenter * presenter = data;

	gtk_widget_hide(presenter->ab_window);
	return TRUE;
}


/* presenter_close */
int presenter_close(Presenter * presenter)
{
	/* FIXME check for unsaved changes */
	presenter_delete(presenter);
	gtk_main_quit();
	return 0;
}


/* presenter_error */
int presenter_error(Presenter * presenter, char const * message, int ret)
{
#if GTK_CHECK_VERSION(2, 18, 0)
	gtk_label_set_text(GTK_LABEL(presenter->infobar_label), message);
	gtk_widget_show(presenter->infobar);
#else
	GtkWidget * dialog;

	dialog = gtk_message_dialog_new(GTK_WINDOW(presenter->window),
			GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR,
			GTK_BUTTONS_CLOSE,
# if GTK_CHECK_VERSION(2, 6, 0)
			"%s", _("Error"));
	gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog),
# endif
			"%s", message);
	gtk_window_set_title(GTK_WINDOW(dialog), _("Error"));
	g_signal_connect(G_OBJECT(dialog), "response", G_CALLBACK(
				gtk_widget_destroy), NULL);
	gtk_widget_show(dialog);
#endif
	return ret;
}


/* presenter_open */
int presenter_open(Presenter * presenter, char const * filename)
{
	if(filename == NULL)
		return presenter_open_dialog(presenter);
	/* FIXME implement */
	return -1;
}


/* presenter_open_dialog */
int presenter_open_dialog(Presenter * presenter)
{
	int ret;
	GtkWidget * dialog;
	GtkFileFilter * filter;
	gchar * filename = NULL;

	dialog = gtk_file_chooser_dialog_new(_("Open file..."),
			GTK_WINDOW(presenter->window),
			GTK_FILE_CHOOSER_ACTION_OPEN,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);
	filter = gtk_file_filter_new();
	gtk_file_filter_set_name(filter, _("Presenter files")); /* XXX */
	gtk_file_filter_add_mime_type(filter, "application/x-presenter");
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);
	filter = gtk_file_filter_new();
	gtk_file_filter_set_name(filter, _("All files"));
	gtk_file_filter_add_pattern(filter, "*");
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);
	if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(
					dialog));
	gtk_widget_destroy(dialog);
	if(filename == NULL)
		return 0;
	ret = presenter_open(presenter, filename);
	g_free(filename);
	return ret;
}


/* private */
/* callbacks */
/* presenter_on_about */
#ifndef EMBEDDED
static void _presenter_on_about(gpointer data)
{
	Presenter * presenter = data;

	presenter_about(presenter);
}
#endif
/* presenter_on_close */
static void _presenter_on_close(gpointer data)
{
	Presenter * presenter = data;

	_presenter_on_closex(presenter);
}


/* presenter_on_closex */
static gboolean _presenter_on_closex(gpointer data)
{
	Presenter * presenter = data;

	if(presenter_close(presenter) == 0)
		return TRUE;
	return FALSE;
}


/* presenter_on_contents */
static void _presenter_on_contents(gpointer data)
{
	desktop_help_contents(PACKAGE, "presenter");
}


/* presenter_on_copy */
static void _presenter_on_copy(gpointer data)
{
	/* FIXME implement */
}


/* presenter_on_cut */
static void _presenter_on_cut(gpointer data)
{
	/* FIXME implement */
}


/* presenter_on_find */
static void _presenter_on_find(gpointer data)
{
	/* FIXME implement */
}


/* presenter_on_insert_file */
static void _presenter_on_insert_file(gpointer data)
{
	/* FIXME implement */
}


/* presenter_on_new */
static void _presenter_on_new(gpointer data)
{
	/* FIXME implement */
}


/* presenter_on_open */
static void _presenter_on_open(gpointer data)
{
	/* FIXME implement */
}


/* presenter_on_paste */
static void _presenter_on_paste(gpointer data)
{
	/* FIXME implement */
}


/* presenter_on_preferences */
static void _presenter_on_preferences(gpointer data)
{
	/* FIXME implement */
}


/* presenter_on_properties */
static void _presenter_on_properties(gpointer data)
{
	/* FIXME implement */
}


/* presenter_on_save */
static void _presenter_on_save(gpointer data)
{
	/* FIXME implement */
}


/* presenter_on_save_as */
static void _presenter_on_save_as(gpointer data)
{
	/* FIXME implement */
}


/* presenter_on_select_all */
static void _presenter_on_select_all(gpointer data)
{
	/* FIXME implement */
}


/* presenter_on_slideshow */
static void _presenter_on_slideshow(gpointer data)
{
	/* FIXME implement */
}


#ifndef EMBEDDED
static void _presenter_on_unselect_all(gpointer data)
{
	/* FIXME implement */
}
#endif
