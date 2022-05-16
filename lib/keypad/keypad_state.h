// -*- coding:utf-8-unix -*-
/*!
    @file       keypad_state.h
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
        limit up to 32 keys.

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#pragma once 
#ifndef file__KEYPAD_STATE_H
#define file__KEYPAD_STATE_H

#include <cstdint> // uint16_t


/// keep state of keyboard
class KeypadState
{
public:
    typedef uint16_t store_t;
    static constexpr uint16_t maximumCapacity = 16; // uint16_t stores 16 bits

    /// Key state
    enum Status : uint8_t
    {
        IDLE     = 0, ///< Key not pressed.      0 == b00
        PRESSED  = 1, ///< Key now is pressed.   1 == b01
        RELEASED = 2, ///< Key now is released.  2 == b10
        HOLD     = 3, ///< Key in pressed state. 3 == b11
    };

private:
    store_t states[2] = {0, 0}; ///< previous [0] an current [1] states of keypad

public:
    void update(store_t state) 
    {
        states[0] = states[1];
        states[1] = state;
    }

    /// is keypad buttons changes?
    bool changed() const { return states[0] != states[1]; }
    
    /// get changes. Bitmask for changed buttons
    store_t changes() const { return states[0] ^ states[1]; }

    /// list (bitmask) of pressed buttons
    store_t pressed() const { return changes() & states[1]; }

    /// list (bitmask) of released buttons
    store_t released() const { return changes() & states[0]; }

    /// return current state
    store_t current() const { return states[1]; }

    /// return previous state
    store_t previous() const { return states[0]; }

    /// is button with specified index pressed?
    bool isPressed(uint8_t buttonIdx) const { return pressed() & ((store_t)1<<buttonIdx); }

    /// return index of first pressed key, -1 of no pressed keys
    int firstPressed() const;

    /// return index of first released key, -1 of no released keys
    int firstReleased() const;

    /// return status if n'th button
    Status statusOf(uint8_t keyIndex) const;

    class Event
    {
    public:
        uint8_t idx;    // button index
        Status status;  // button status
    };

    class EventStream
    {
        //friend class KeypadState;
        store_t states[3]; // prev, curr, changes;
        uint8_t pos; // current key index

    public:
        EventStream(KeypadState const& st);

        /// no more events in stream
        bool empty() const { return states[2] == 0; }

        /// pop event
        Event pop();
    };
};

#endif // file__KEYPAD_STATE_H