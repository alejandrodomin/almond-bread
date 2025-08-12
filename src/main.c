#include <stdbool.h>
#include <stdio.h>

int brot_point(double real, double imag);
double coord_trnsfrm(unsigned int index, unsigned int size, double bot_bound, double up_bound);
char* color(unsigned int count);

int main(void) {
    // init
    const int width = 100;
    const int height = 50;
    const char* set[height][width];

    double real_min = -1.5, real_max = 0.5;
    double imag_min = -1.0, imag_max = 1.0;

    while (true) {
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
        printf("hit enter to submit choice:\n");
        printf("? q - quit, h - left, j - down, k - up, l - right, + - zoom in, - - zoom in\r");
        char cmd = (char)getchar();

        double real_range = real_max - real_min;
        double imag_range = imag_max - imag_min;

        double real_change = real_range * .1;
        double imag_change = imag_range * .1;
        switch (cmd) {
            case 'q':
                return 0;
                break;
            case 'j':
                imag_min -= imag_change;
                imag_max -= imag_change;
                break;
            case 'k':
                imag_min += imag_change;
                imag_max += imag_change;
                break;
            case 'h':
                real_min -= real_change;
                real_max -= real_change;
                break;
            case 'l':
                real_min += real_change;
                real_max += real_change;
                break;
            case '+':
                real_min += real_change;
                real_max -= real_change;

                imag_min += imag_change;
                imag_max -= imag_change;
                break;
            case '-':
                real_min -= real_change;
                real_max += real_change;

                imag_min -= imag_change;
                imag_max += imag_change;
                break;
        }
    }

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

double coord_trnsfrm(unsigned int index, unsigned int size, double bot_bound, double up_bound) {
    double percent = index / (double)(size - 1);
    double range = up_bound - bot_bound;

    return (percent * range) + bot_bound;
}

char* color(unsigned int count) {
    switch (count) {
        case 0:
            return "\033[0;40m";  // Black block (in set)
            break;
        case 1:
            return "\033[0;41m";
            break;
        case 2:
            return "\033[0;42m";
            break;
        case 3:
            return "\033[0;43m";
            break;
        case 4:
            return "\033[0;44m";
            break;
        case 5:
            return "\033[0;45m";
            break;
        case 6:
            return "\033[0;46m";
            break;
        default:
            return "\033[0;47m";  // White block (outside)
            break;
    }
}
