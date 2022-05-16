#pragma once
#ifndef file__PWMOUT_X_H
#define file__PWMOUT_X_H

#include "PwmOut.h"


class PwmOutX : public mbed::PwmOut
{
public:
    PwmOutX(PinName pin);
    ~PwmOutX();

    void stop();
    void start();
    void pulse_width_us(uint32_t pulse);
    void set_reprtition(uint32_t rep);

    uint32_t count() const;

protected:

};

#endif // file__PWMOUT_X_H
