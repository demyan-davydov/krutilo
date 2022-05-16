// -*- coding:utf-8-unix -*-
/*!
    @file       keypad_state.cpp
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-10-23

    @brief      Part of Keypad support library
    @description
        This class keep keypad state, actually 2 states: for previous
        update and current state. Each of states keeps in uint16 variable
        where each bit means: 0 - not pushed, 1 - pushed.
        So, every time we are can decide what event are happens for every 
        button: 
          if previos state = 0 and current state = 0: button is not pressed
          if previos state = 0 and current state = 1: button was pressed
          if previos state = 1 and current state = 0: button was released
          if previos state = 1 and current state = 1: button is holded
        As we use uint16_t variable -- maximum 16 buttons are supported,
        you may change typedef of 'store_t' to uint32_t to expand this 
        limit up to 32 keys

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#include "keypad_state.h"


namespace 
{
    // CTZ -- count of trailing zeroes
    // return number of zeroes before first significant bit
    // for example: 
    //   b0001 -> 0
    //   b0010 -> 1
    //   b0100 -> 2 ...
    // If value is 0 returned 0, but it is not important for our purposes
    // definitions for uint16_t and for uint32_t

    // todo: use __builtin_ctz() for gcc/clang or ffs() from <strings.h>?

    template<typename T>
    uint8_t ctz(T x);

    template<> uint8_t ctz<uint16_t>(uint16_t x)
    {
        uint8_t n = 0;
        if ((x & 0x000000FF) == 0) { n = n +  8; x = x >>  8; }
        if ((x & 0x0000000F) == 0) { n = n +  4; x = x >>  4; }
        if ((x & 0x00000003) == 0) { n = n +  2; x = x >>  2; }
        if ((x & 0x00000001) == 0) { n = n +  1; }
        return n;
    }

    template<> uint8_t ctz<uint32_t>(uint32_t x)
    { 
        uint8_t n = 0;
        if ((x & 0x0000FFFF) == 0) { n = n + 16; x = x >> 16; }
        if ((x & 0x000000FF) == 0) { n = n +  8; x = x >>  8; }
        if ((x & 0x0000000F) == 0) { n = n +  4; x = x >>  4; }
        if ((x & 0x00000003) == 0) { n = n +  2; x = x >>  2; }
        if ((x & 0x00000001) == 0) { n = n +  1; }
        return n;
    }

    /*static uint16_t is_power_of_2(uint16_t x) 
    { 
        return (x && !(x & x-1)); 
    }

    static uint32_t is_power_of_2(uint32_t x) 
    { 
        return (x && !(x & x-1)); 
    }*/
} // anonimous namespace


int KeypadState::firstPressed() const
{
    const auto e = pressed();
    return e ? ctz(e) : -1;
}

int KeypadState::firstReleased() const
{
    const auto e = released();
    return e ? ctz(e) : -1;
}

KeypadState::Status KeypadState::statusOf(uint8_t keyIndex) const
{
    const store_t prev = (states[0] >> keyIndex) & 1;
    const store_t curr = (states[1] >> keyIndex) & 1;
    return (Status)(((prev & 1) << 1) | (curr & 1));
}

KeypadState::EventStream::EventStream(KeypadState const& st)
: states{st.previous(), st.current(), st.changes()}
, pos(0)
{
}

KeypadState::Event KeypadState::EventStream::pop()
{
    // take 1st event
    const uint8_t bitPos = ctz(states[2]);

    states[0] >>= bitPos;
    states[1] >>= bitPos;
    states[2] >>= bitPos;

    const auto st = (Status)(((states[0] & 1) << 1) | (states[1] & 1));
    
    states[0] >>= 1;
    states[1] >>= 1;
    states[2] >>= 1;
    pos += 1 + bitPos;

    return Event{(uint8_t)(pos-1), st};
}

