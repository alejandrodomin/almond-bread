#include <stdbool.h>
#include <stdio.h>

#include "termios-util.h"
#include "util.h"

int brot_point(double real, double imag);

double real_min = -1.5, real_max = 0.5;
double imag_min = -1.0, imag_max = 1.0;

int main(void) {
    enable_raw_mode();

    // init
    const int width = 100;
    const int height = 50;
    const char* set[height][width];

    bool run = true;
    while (run) {
        // clear term
        printf("\033[H");

        // gen set
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                double real = coord_trnsfrm(x, width, real_min, real_max);
                double imag = coord_trnsfrm(y, height, imag_min, imag_max);

                unsigned int count = brot_point(real, imag);
                set[y][x] = color(count);
            }
        }

        // print set
        for (int y = height - 1; y >= 0; y--) {
            for (int x = 0; x < width; x++) {
                printf("%s \033[0m", set[y][x]);
            }
            printf("\n");
        }

        // input
        run = take_input();
    }

    disable_raw_mode();
    return 0;
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
