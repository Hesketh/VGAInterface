#ifndef _MATH_H
#define _MATH_H

#include <stdint.h>

#define PI 3.14159

// Returns the absolute value of a number
uint16_t abs(uint16_t value);

// Returns the cosine of the given value
double cos(double value);

// Returns the sin of the given value
double sin(double value);

// For some reason Bochs triggers a math abort when using these methods
// Commented out until I can figure out how to fix it

// Rounds value upwards
//double ceil(double value);

// Rounds value downwards
//double floor(double value);

#endif