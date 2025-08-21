#ifndef GTK_UTIL_H
#define GTK_UTIL_H

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtk.h>

void activate(GtkApplication* app, gpointer user_data);
void draw(GtkDrawingArea* area, cairo_t* cr, int width, int height, gpointer user_data);
gboolean on_scroll_event(GtkEventControllerScroll* controller, double dx, double dy, gpointer user_data);

#endif
