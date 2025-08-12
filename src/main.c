#include <coz.h>
#include <stdbool.h>
#include <stdio.h>

int brot_point(double x, double y);

int main(void) {
    const int width = 100;
    const int height = 50;
    const char* set[height][width];

    double real_min = -1.5, real_max = 0.5;
    double imag_min = -1.0, imag_max = 1.0;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double real = real_min + (x / (double)(width - 1)) * (real_max - real_min);
            double imag = imag_min + (y / (double)(height - 1)) * (imag_max - imag_min);

            switch (brot_point(real, imag)) {
                case 0:
                    set[y][x] = "\033[0;40m";  // Black block (in set)
                    break;
                case 1:
                    set[y][x] = "\033[0;41m";
                    break;
                case 2:
                    set[y][x] = "\033[0;42m";
                    break;
                case 3:
                    set[y][x] = "\033[0;43m";
                    break;
                case 4:
                    set[y][x] = "\033[0;44m";
                    break;
                case 5:
                    set[y][x] = "\033[0;45m";
                    break;
                case 6:
                    set[y][x] = "\033[0;46m";
                    break;
                default:
                    set[y][x] = "\033[0;47m";  // White block (outside)
                    break;
            }
        }
    }

    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            printf("%s \033[0m", set[y][x]);
        }
        printf("\n");
    }

    return 0;
}

int brot_point(double x, double y) {
    double a = 0.0, b = 0.0;
    int count = 0;

    while (count < 100) {
        double a_n = a * a - b * b + x;
        double b_n = 2 * a * b + y;

        a = a_n;
        b = b_n;

        if (a * a + b * b > 4.0) {
            return count;
        }

        count++;
    }

    return 0;
}
