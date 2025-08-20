#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdio.h>

#include "util.h"

int brot_point(double real, double imag);

double real_min = -1.5, real_max = 0.5;
double imag_min = -1.0, imag_max = 1.0;

static void draw(GtkDrawingArea* area, cairo_t* cr, int width, int height, gpointer user_data) {
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
            if (count == 0) {
                val = 255;
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

static void activate(GtkApplication* app, gpointer user_data) {
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

    // link and show win
    gtk_window_set_child(GTK_WINDOW(window), draw_area);
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char* argv[]) {
    // init window
    GtkApplication* app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

// void misc() {
//     enable_raw_mode();
//     // init
//     const char* set[HEIGHT][WIDTH];
//     // main loop
//     bool run = true;
//     while (run) {
//         // clear term
//         printf("\033[H");
//
//         // gen set
//         for (int y = 0; y < HEIGHT; y++) {
//             for (int x = 0; x < WIDTH; x++) {
//                 double real = coord_trnsfrm(x, WIDTH, real_min, real_max);
//                 double imag = coord_trnsfrm(y, HEIGHT, imag_min, imag_max);
//
//                 unsigned int count = brot_point(real, imag);
//                 set[y][x] = color(count);
//             }
//         }
//
//         // print set
//         for (int y = HEIGHT - 1; y >= 0; y--) {
//             for (int x = 0; x < WIDTH; x++) {
//                 printf("%s \033[0m", set[y][x]);
//             }
//             printf("\n");
//         }
//
//         // input
//         run = take_input();
//     }
//
//     disable_raw_mode();
// }

int brot_point(double real, double imag) {
    double a = 0.0, b = 0.0;
    int count = 0;

    while (count < 100) {
        double a_n = a * a - b * b + real;
        double b_n = 2 * a * b + imag;

        a = a_n;
        b = b_n;

        if (a * a + b * b > 4.0) {
            return count;
        }

        count++;
    }

    return 0;
}
