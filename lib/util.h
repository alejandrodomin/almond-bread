#ifndef UTIL_H
#define UTIL_H

extern double real_min, real_max, imag_min, imag_max;

int brot_point(double real, double imag);
double coord_trnsfrm(unsigned int index, unsigned int size, double bot_bound, double up_bound);
char* color(unsigned int count);
bool take_input();

#endif
