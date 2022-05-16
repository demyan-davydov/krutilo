// -*- coding:utf-8-unix -*-
/*!
    @file       keypad.h
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-10-23

    @brief      Keypad support library
    @description
        This library provides a interface for using matrix keypads. 
        It supports multiple keypresses and checking status of each 
        button.

        Supported keypads up to 16 buttons, this limit may be easily 
        expanded up to 32 buttons and not so easily -- up to any buttons.
        To realise button limit, see KeypadState class.
        
        Note: This implementation use rows as output pins and 
        columns as input pins. Input pins are pulled up via 
        internal resistors, but highly recommended also add external
        pullup resistors by 10KOmh to column pins, it prevent false 
        buttons pushes.

        Indexing: Buttons numerated by rows, indexing from 0. 
        So, button in 0-row and 0-columt have index 0, button in
        R-row and C-column have index: R * numbers_of_columns + C

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#pragma once
#ifndef file__KEYPAD_H
#define file__KEYPAD_H

#include "mbed.h"
#include <cstdint> // uint32_t
#include <vector>
#include "keypad_state.h"


class Keypad
{
public: 
    // Dummy names, for using list initializer {} in c'tor
    // for example: 
    // Keypad keypad({1,2,3,4}, {5,6,7}); // initialize Keypad 4x3: 4 rows, 3 columns
    
    struct PinNames1 { PinName names[1]; };
    struct PinNames2 { PinName names[2]; };
    struct PinNames3 { PinName names[3]; };
    struct PinNames4 { PinName names[4]; };

private:
    void construct(PinName const* rowPins, PinName const* columnPins, size_t nRows, size_t nColumns);

public:
    typedef uint32_t Timestamp; ///< timestamp to store count of milliseconds


    /// c'tor 3x3 keypad
    Keypad(PinNames3 rowPins, PinNames3 colPins, int minUpdateTimeMs = 10);

    /// c'tor 4x4 keypad
    Keypad(PinNames4 rowPins, PinNames4 colPins, int minUpdateTimeMs = 10);

    /// c'tor 3x4 keypad
    Keypad(PinNames3 rowPins, PinNames4 colPins, int minUpdateTimeMs = 10);

    /// c'tor 4x3 keypad
    Keypad(PinNames4 rowPins, PinNames3 colPins, int minUpdateTimeMs = 10);

    /// Add here more c'tors if you need


    /// init updation
    /// recommend to call it before first update() call 
    /// this method initiate debounce array
    void init(Timestamp timeMs);

    /// Scan keypad and update key states
    /// @return true if state changed
    bool update(Timestamp timeMs);

    /// total count of buttons
    uint8_t buttonsCount() const { return uint8_t(m_rowPins.size() * m_colPins.size()); }

    /// Count of button rows
    uint8_t buttonRows() const { return (uint8_t)m_rowPins.size(); }

    /// Count of button columns
    uint8_t buttonCols() const { return (uint8_t)m_colPins.size(); }

    /// get status of Nth button. Status is (IDLE,PRESSED,RELEASED,HOLD)
    KeypadState::Status statusOf(uint8_t idx) const
    {
        return state().statusOf(idx);
    }

    /// get status of [column, row] button. Status is (IDLE,PRESSED,RELEASED,HOLD)
    KeypadState::Status statusOf(uint8_t column, uint8_t row) const
    {
        return statusOf( column * buttonCols() + row );
    }

    /// return current keypad state
    /// allow to get button states and events on buttons
    KeypadState const& state() const { return m_state; }

private:
    // Not best idea use std::vector, but i am lazy
    std::vector<DigitalOut> m_rowPins; ///< Row pins
    std::vector<DigitalIn> m_colPins; ///< Column pins

    Timestamp m_minUpdateTimeMs;      ///< limit to prevent too often updates
    Timestamp m_lastUpdateTimeMs = 0; ///< last time then update() methos was called

    KeypadState m_state; //< last updated button state
    Timestamp m_updateTimes[KeypadState::maximumCapacity] = {0}; ///< Table of update times for each button. For debouncing
    Timestamp m_debouncingMs = 60; ///< Debouncing time ms
};


#endif // #ifndef file__KEYPAD_H
