#pragma once 
#ifndef file__MATH_UTIL_H
#define file__MATH_UTIL_H


#define my_PI 3.141592653589793238463
#define my_PIx2 6.28318530718
#define my_PI_2 1.57079632679

// slow sin
// standard sin() function takes too much space 
float sinx(float x);

/// slow but compact cos
float cosx(float x);

#endif // file__MATH_UTIL_H
