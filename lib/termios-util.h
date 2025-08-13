#ifndef TERMIOS_UTIL_H
#define TERMIOS_UTIL_H

#include <termios.h>
#include <unistd.h>

void enable_raw_mode();
void disable_raw_mode();

#endif
