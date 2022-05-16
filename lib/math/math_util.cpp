#include "math_util.h"

float sinx(float x)
{
    float n = x;
    float sum = 0.0;
    int i = 1;
 
    do {
        sum += n;
        n *= -1.0f * x * x / ((2 * i) * (2 * i + 1));
        ++i;
    }
    while ((n > 0.0 ? n : -n) > 1E-7);
 
    return sum;
}

float cosx(float x)
{
    return sinx(x+my_PI_2);
}

