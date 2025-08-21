#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdio.h>

#include "gtk-util.h"
#include "util.h"

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
