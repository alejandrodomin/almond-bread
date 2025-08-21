#include "gtk-util.h"

#include "util.h"

double real_min = -1.5, real_max = 0.5;
double imag_min = -1.0, imag_max = 1.0;

void activate(GtkApplication* app, gpointer user_data) {
    GtkWidget* window = gtk_application_window_new(app);

    const int default_size = 500;
    gtk_window_set_title(GTK_WINDOW(window), "AlmondBread Set");
    gtk_window_set_default_size(GTK_WINDOW(window), default_size, default_size);

    // drawing area init
    GtkWidget* draw_area = gtk_drawing_area_new();
    GdkPixbuf* pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, default_size, default_size);
    if (!pixbuf) {
        g_error("Failed to create GdkPixbuf.");
        return;
    }

    gtk_drawing_area_set_content_width(GTK_DRAWING_AREA(draw_area), default_size);
    gtk_drawing_area_set_content_height(GTK_DRAWING_AREA(draw_area), default_size);
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(draw_area), draw, pixbuf, nullptr);

    // mouse input init
    GtkEventController* scroll = gtk_event_controller_scroll_new(GTK_EVENT_CONTROLLER_SCROLL_VERTICAL);
    g_signal_connect(scroll, "scroll", G_CALLBACK(on_scroll_event), draw_area);
    gtk_widget_add_controller(GTK_WIDGET(draw_area), scroll);

    // link and show win
    gtk_window_set_child(GTK_WINDOW(window), draw_area);
    gtk_window_present(GTK_WINDOW(window));
}

void draw(GtkDrawingArea* area, cairo_t* cr, int width, int height, gpointer user_data) {
    GdkPixbuf* pixbuf = user_data;
    if (!GDK_IS_PIXBUF(pixbuf)) {
        g_warning("Invalid pixbuf in draw(): %p", pixbuf);
        return;
    }
    int curr_width = gdk_pixbuf_get_width(pixbuf);
    int curr_height = gdk_pixbuf_get_height(pixbuf);
    if (curr_width != width && curr_height != height) {
        g_object_unref(pixbuf);
        pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, width, height);
    }

    // 1) Modify pixel data BEFORE drawing
    int img_width = gdk_pixbuf_get_width(pixbuf);
    int img_height = gdk_pixbuf_get_height(pixbuf);
    int n_channels = gdk_pixbuf_get_n_channels(pixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(pixbuf);
    guchar* pixels = gdk_pixbuf_get_pixels(pixbuf);

    for (int y = 0; y < img_height; y++) {
        guchar* row = pixels + y * rowstride;
        for (int x = 0; x < img_width; x++) {
            double real = coord_trnsfrm(x, width, real_min, real_max);
            double imag = coord_trnsfrm(y, height, imag_min, imag_max);

            unsigned int count = brot_point(real, imag);

            int val = 0;
            if (count > 0) {
                val = ((double)count / 255) * 255;
            }

            guchar* p = row + x * n_channels;
            p[0] = val;                       // red
            p[1] = val;                       // green
            p[2] = val;                       // blue
            if (n_channels == 4) p[3] = 255;  // alpha
        }
    }

    // 2) Draw the modified pixbuf in one operation
    gdk_cairo_set_source_pixbuf(cr, pixbuf, 0, 0);
    cairo_paint(cr);
}

gboolean on_scroll_event(GtkEventControllerScroll* controller, double dx, double dy, gpointer user_data) {
    GtkWidget* draw_area = (GtkWidget*)user_data;

    if (dy < 0) {
        // zoom in
        real_min /= 1.1;
        real_max /= 1.1;
        imag_min /= 1.1;
        imag_max /= 1.1;
    } else if (dy > 0) {
        real_min *= 1.1;
        real_max *= 1.1;
        imag_min *= 1.1;
        imag_max *= 1.1;
    }

    gtk_widget_queue_draw(draw_area);
    return GDK_EVENT_STOP;
}
