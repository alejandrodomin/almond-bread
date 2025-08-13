#include "util.h"

#include <stdio.h>
#include <unistd.h>

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

bool take_input() {
    printf("hit enter to submit choice:\n");
    printf("? q - quit, h - left, j - down, k - up, l - right, + - zoom in, - - zoom in\r");
    char cmd;
    read(STDIN_FILENO, &cmd, 1);

    double real_range = real_max - real_min;
    double imag_range = imag_max - imag_min;

    double real_change = real_range * .1;
    double imag_change = imag_range * .1;
    switch (cmd) {
        case 'q':
            return false;
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

    return true;
}
