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


#include <stdarg.h>
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

	/* preferences */
	Config * config;
	int monitor;

	/* widgets */
	GtkWidget * window;
	GtkWidget * view;
	GtkWidget * statusbar;
#if GTK_CHECK_VERSION(2, 18, 0)
	/* infobar */
	GtkWidget * infobar;
	GtkWidget * infobar_label;
#endif
	/* find */
	GtkWidget * fi_dialog;
	GtkListStore * fi_store;
	GtkWidget * fi_text;
	/* about */
	GtkWidget * ab_window;

	/* properties */
	GtkWidget * pp_window;

	/* preferences */
	GtkWidget * pr_window;

	/* slideshow */
	GtkWidget * sl_window;
	GtkWidget * sl_toolbar;
	GtkWidget * sl_title;
};


/* prototypes */
/* useful */
static void _presenter_present(Presenter * presenter);

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
static void _presenter_on_slideshow_close(gpointer data);
static gboolean _presenter_on_slideshow_closex(gpointer data);
static void _presenter_on_slideshow_next(gpointer data);
static void _presenter_on_slideshow_previous(gpointer data);
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

static const DesktopAccel _presenter_accel_slideshow[] =
{
	{ G_CALLBACK(_presenter_on_slideshow_close), 0, GDK_KEY_Escape },
	{ NULL, 0, 0 }
};

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
	presenter->window = NULL;
	/* check for errors */
	if(presenter->config == NULL)
	{
		object_delete(presenter);
		return NULL;
	}
	/* FIXME load the configuration */
	presenter->monitor = -1;
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
	/* about */
	presenter->ab_window = NULL;
	/* properties */
	presenter->pp_window = NULL;
	/* preferences */
	presenter->pr_window = NULL;
	/* slideshow */
	presenter->sl_window = NULL;
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
	if(presenter->window != NULL)
		gtk_widget_destroy(presenter->window);
	if(presenter->config != NULL)
		config_delete(presenter->config);
	object_delete(presenter);
}


/* accessors */
/* presenter_set_monitor */
int presenter_set_monitor(Presenter * presenter, int monitor)
{
	GdkScreen * screen;

	if(monitor < 0)
	{
		presenter->monitor = -1;
		return 0;
	}
	screen = gdk_screen_get_default();
	if(monitor >= gdk_screen_get_n_monitors(screen))
		return -1;
	presenter->monitor = monitor;
	return 0;
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
	int res;

	/* FIXME really check for unsaved changes */
	res = presenter_confirm(presenter, _("There are unsaved changes.\n"
				"Discard or save them?"),
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			GTK_STOCK_DISCARD, GTK_RESPONSE_REJECT,
			GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT, NULL);
	if(res == GTK_RESPONSE_CANCEL || res == GTK_RESPONSE_DELETE_EVENT)
		return 1;
	else if(res == GTK_RESPONSE_ACCEPT && presenter_save(presenter) != TRUE)
		return 1;
	gtk_widget_hide(presenter->window);
	gtk_main_quit();
	return 0;
}


/* presenter_confirm */
int presenter_confirm(Presenter * presenter, char const * message, ...)
{
	GtkWidget * dialog;
	va_list ap;
	char const * action;
	int res;

	dialog = gtk_message_dialog_new(GTK_WINDOW(presenter->window),
			GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_QUESTION,
			GTK_BUTTONS_NONE,
#if GTK_CHECK_VERSION(2, 6, 0)
			"%s", _("Question"));
	gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog),
#endif
			"%s", message);
	va_start(ap, message);
	while((action = va_arg(ap, char const *)) != NULL)
		gtk_dialog_add_button(GTK_DIALOG(dialog),
				action, va_arg(ap, int));
	va_end(ap);
	gtk_window_set_title(GTK_WINDOW(dialog), _("Question"));
	res = gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	return res;
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


/* presenter_save */
int presenter_save(Presenter * presenter)
{
	if(presenter->filename == NULL)
		return presenter_save_as_dialog(presenter);
	/* FIXME implement */
	return -1;
}


/* presenter_save_as */
int presenter_save_as(Presenter * presenter, char const * filename)
{
	if(filename == NULL)
		return presenter_save_as_dialog(presenter);
	/* FIXME implement */
	return -1;
}


/* presenter_save_as_dialog */
int presenter_save_as_dialog(Presenter * presenter)
{
	int ret;
	GtkWidget * dialog;
	gchar * filename = NULL;

	dialog = gtk_file_chooser_dialog_new(_("Save as..."),
			GTK_WINDOW(presenter->window),
			GTK_FILE_CHOOSER_ACTION_SAVE,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT, NULL);
	if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(
					dialog));
	gtk_widget_destroy(dialog);
	if(filename == NULL)
		return 0;
	ret = presenter_save_as(presenter, filename);
	g_free(filename);
	return ret;
}


/* presenter_show_preferences */
static void _preferences_set(Presenter * presenter);
static void _preferences_window(Presenter * presenter);
/* callbacks */
static void _preferences_on_apply(gpointer data);
static void _preferences_on_cancel(gpointer data);
static gboolean _preferences_on_closex(gpointer data);
static void _preferences_on_ok(gpointer data);
static void _preferences_on_response(GtkWidget * widget, gint response,
		gpointer data);

void presenter_show_preferences(Presenter * presenter, gboolean show)
{
	if(presenter->pr_window == NULL)
		_preferences_window(presenter);
	if(show)
		gtk_window_present(GTK_WINDOW(presenter->pr_window));
	else
		gtk_widget_hide(presenter->pr_window);
}

static void _preferences_set(Presenter * presenter)
{
	/* FIXME implement */
}

static void _preferences_window(Presenter * presenter)
{
	GtkWidget * vbox;

	presenter->pr_window = gtk_dialog_new_with_buttons(
			_("Presentation tool preferences"),
			GTK_WINDOW(presenter->window),
			GTK_DIALOG_DESTROY_WITH_PARENT,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			GTK_STOCK_APPLY, GTK_RESPONSE_APPLY,
			GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);
	g_signal_connect_swapped(presenter->pr_window, "delete-event",
			G_CALLBACK(_preferences_on_closex), presenter);
	g_signal_connect(presenter->pr_window, "response", G_CALLBACK(
				_preferences_on_response), presenter);
#if GTK_CHECK_VERSION(2, 14, 0)
	vbox = gtk_dialog_get_content_area(GTK_DIALOG(presenter->pr_window));
#else
	vbox = GTK_DIALOG(player->pr_window)->vbox;
#endif
	gtk_box_set_spacing(GTK_BOX(vbox), 4);
	/* FIXME implement */
	_preferences_set(presenter);
	gtk_widget_show_all(vbox);
}

static void _preferences_on_apply(gpointer data)
{
	/* FIXME implement */
}

static void _preferences_on_cancel(gpointer data)
{
	Presenter * presenter = data;

	_preferences_set(presenter);
}

static gboolean _preferences_on_closex(gpointer data)
{
	Presenter * presenter = data;

	gtk_widget_hide(presenter->pr_window);
	_preferences_on_cancel(presenter);
	return TRUE;
}

static void _preferences_on_ok(gpointer data)
{
	Presenter * presenter = data;

	_preferences_on_apply(presenter);
	/* FIXME fully implement (save...) */
}

static void _preferences_on_response(GtkWidget * widget, gint response,
		gpointer data)
{
	Presenter * presenter = data;

	switch(response)
	{
		case GTK_RESPONSE_APPLY:
			_preferences_on_apply(presenter);
			break;
		case GTK_RESPONSE_CANCEL:
			gtk_widget_hide(presenter->pr_window);
			_preferences_on_cancel(presenter);
			break;
		case GTK_RESPONSE_OK:
			gtk_widget_hide(presenter->pr_window);
			_preferences_on_ok(presenter);
			break;
	}
}


/* private */
/* useful */
/* presenter_present */
static void _present_window(Presenter * presenter);

static void _presenter_present(Presenter * presenter)
{
	GdkScreen * screen;
	int monitor = 0;
	GdkRectangle rect;

	if(presenter->sl_window == NULL)
		_present_window(presenter);
	/* (re-)configure the window */
	screen = gdk_screen_get_default();
	if(presenter->monitor >= 0 && presenter->monitor
			< gdk_screen_get_n_monitors(screen))
		monitor = presenter->monitor;
	gdk_screen_get_monitor_geometry(screen, monitor, &rect);
	gtk_window_move(GTK_WINDOW(presenter->sl_window), rect.x, rect.y);
	gtk_window_resize(GTK_WINDOW(presenter->sl_window), rect.width,
			rect.height);
	gtk_widget_show(presenter->sl_window);
	gtk_window_fullscreen(GTK_WINDOW(presenter->sl_window));
}

static void _present_window(Presenter * presenter)
{
	PangoFontDescription * font;
	GtkAccelGroup * group;
	GdkColor black = { 0x0, 0x0000, 0x0000, 0x0000 };
	GdkColor white = { 0x0, 0xffff, 0xffff, 0xffff };
	GtkWidget * vbox;
	GtkToolItem * toolitem;

	/* font */
	font = pango_font_description_new();
	pango_font_description_set_size(font, 20 * PANGO_SCALE);
	pango_font_description_set_weight(font, PANGO_WEIGHT_BOLD);
	/* window */
	group = gtk_accel_group_new();
	presenter->sl_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_add_accel_group(GTK_WINDOW(presenter->sl_window), group);
	desktop_accel_create(_presenter_accel_slideshow, presenter, group);
	g_object_unref(group);
	gtk_window_set_keep_above(GTK_WINDOW(presenter->sl_window), TRUE);
	gtk_widget_modify_bg(presenter->sl_window, GTK_STATE_NORMAL, &black);
	g_signal_connect_swapped(presenter->sl_window, "delete-event",
			G_CALLBACK(_presenter_on_slideshow_closex), presenter);
	vbox = gtk_vbox_new(FALSE, 0);
	/* toolbar */
	presenter->sl_toolbar = gtk_toolbar_new();
	toolitem = gtk_tool_button_new_from_stock(GTK_STOCK_GO_BACK);
	g_signal_connect_swapped(toolitem, "clicked", G_CALLBACK(
				_presenter_on_slideshow_previous), presenter);
	gtk_toolbar_insert(GTK_TOOLBAR(presenter->sl_toolbar), toolitem, -1);
	toolitem = gtk_tool_button_new_from_stock(GTK_STOCK_GO_FORWARD);
	g_signal_connect_swapped(toolitem, "clicked", G_CALLBACK(
				_presenter_on_slideshow_next), presenter);
	gtk_toolbar_insert(GTK_TOOLBAR(presenter->sl_toolbar), toolitem, -1);
	toolitem = gtk_separator_tool_item_new();
	gtk_toolbar_insert(GTK_TOOLBAR(presenter->sl_toolbar), toolitem, -1);
	toolitem = gtk_tool_button_new_from_stock(GTK_STOCK_QUIT);
	g_signal_connect_swapped(toolitem, "clicked", G_CALLBACK(
				_presenter_on_slideshow_close), presenter);
	gtk_toolbar_insert(GTK_TOOLBAR(presenter->sl_toolbar), toolitem, -1);
	gtk_box_pack_start(GTK_BOX(vbox), presenter->sl_toolbar, FALSE, TRUE,
			0);
	/* title */
	/* FIXME really implement */
	presenter->sl_title = gtk_label_new("<Insert title here>");
	gtk_widget_modify_fg(presenter->sl_title, GTK_STATE_NORMAL, &white);
	gtk_widget_modify_font(presenter->sl_title, font);
	gtk_box_pack_start(GTK_BOX(vbox), presenter->sl_title, FALSE, TRUE,
			0);
	gtk_container_add(GTK_CONTAINER(presenter->sl_window), vbox);
	gtk_widget_show_all(vbox);
	pango_font_description_free(font);
}


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

	presenter_close(presenter);
}


/* presenter_on_closex */
static gboolean _presenter_on_closex(gpointer data)
{
	Presenter * presenter = data;

	_presenter_on_close(presenter);
	return TRUE;
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
	Presenter * presenter = data;

	presenter_open_dialog(presenter);
}


/* presenter_on_paste */
static void _presenter_on_paste(gpointer data)
{
	/* FIXME implement */
}


/* presenter_on_preferences */
static void _presenter_on_preferences(gpointer data)
{
	Presenter * presenter = data;

	presenter_show_preferences(presenter, TRUE);
}


/* presenter_on_properties */
static void _presenter_on_properties(gpointer data)
{
	/* FIXME implement */
}


/* presenter_on_save */
static void _presenter_on_save(gpointer data)
{
	Presenter * presenter = data;

	presenter_save(presenter);
}


/* presenter_on_save_as */
static void _presenter_on_save_as(gpointer data)
{
	Presenter * presenter = data;

	presenter_save_as_dialog(presenter);
}


/* presenter_on_select_all */
static void _presenter_on_select_all(gpointer data)
{
	/* FIXME implement */
}


/* presenter_on_slideshow */
static void _presenter_on_slideshow(gpointer data)
{
	Presenter * presenter = data;

	_presenter_present(presenter);
}


/* presenter_on_slideshow_close */
static void _presenter_on_slideshow_close(gpointer data)
{
	Presenter * presenter = data;

	gtk_widget_hide(presenter->sl_window);
}


/* presenter_on_slideshow_closex */
static gboolean _presenter_on_slideshow_closex(gpointer data)
{
	Presenter * presenter = data;

	_presenter_on_slideshow_close(presenter);
	return TRUE;
}


/* presenter_on_slideshow_next */
static void _presenter_on_slideshow_next(gpointer data)
{
	/* FIXME implement */
}


/* presenter_on_slideshow_previous */
static void _presenter_on_slideshow_previous(gpointer data)
{
	/* FIXME implement */
}


#ifndef EMBEDDED
static void _presenter_on_unselect_all(gpointer data)
{
	/* FIXME implement */
}
#endif
