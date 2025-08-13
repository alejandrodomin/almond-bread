#include "termios-util.h"

struct termios orig_termios;

void disable_raw_mode() { tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios); }

void enable_raw_mode() {
    // Get original terminal attributes
    tcgetattr(STDIN_FILENO, &orig_termios);

    struct termios raw = orig_termios;

    // Input flags - Turn off canonical mode and echo
    raw.c_lflag &= ~(ICANON | ECHO);

    // Apply new attributes
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
