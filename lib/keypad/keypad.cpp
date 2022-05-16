// -*- coding:utf-8-unix -*-
/*!
    @file       keypad.cpp
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

#include "keypad.h"


void Keypad::construct(PinName const* rowPins, PinName const* columnPins, size_t nRows, size_t nColumns)
{
    /*if (nRows * nColumns > KeypadState::maximumCapacity)
    {
        throw std::invalid_argument("too much buttons");
    }*/

    // naive failure protection
    // this class limited for 'KeypadState::maximumCapacity' buttons
    if (nRows > 4) nRows = 4;
    if (nColumns > 4) nColumns = 4;

    // Configure rows for Output
    m_rowPins.reserve(nRows);
    for (int i = 0; i < (int)nRows; ++i) {
        m_rowPins.emplace_back(DigitalOut(rowPins[i], 1));
    }

    // Configure columns for Input
    m_colPins.reserve(nColumns);
    for (int i = 0; i < (int)nColumns; ++i) {
        m_colPins.emplace_back(DigitalIn(columnPins[i], PinMode::PullUp));
    }
}

Keypad::Keypad(PinNames3 rowPins, PinNames3 colPins, int minUpdateTimeMs) : m_minUpdateTimeMs(minUpdateTimeMs)
{
    construct(rowPins.names, colPins.names, 3, 3);
}

Keypad::Keypad(PinNames4 rowPins, PinNames4 colPins, int minUpdateTimeMs) : m_minUpdateTimeMs(minUpdateTimeMs)
{
    construct(rowPins.names, colPins.names, 4, 4);
}

Keypad::Keypad(PinNames3 rowPins, PinNames4 colPins, int minUpdateTimeMs) : m_minUpdateTimeMs(minUpdateTimeMs)
{
    construct(rowPins.names, colPins.names, 3, 4);
}

Keypad::Keypad(PinNames4 rowPins, PinNames3 colPins, int minUpdateTimeMs) : m_minUpdateTimeMs(minUpdateTimeMs)
{
    construct(rowPins.names, colPins.names, 4, 3);
}

void Keypad::init(Timestamp t)
{
    m_lastUpdateTimeMs = t;
    for (int i = 0; i < KeypadState::maximumCapacity; ++i) {
        m_updateTimes[i] = t;
    }
}

bool Keypad::update(Timestamp t)
{
    if(t - m_lastUpdateTimeMs < m_minUpdateTimeMs) {
        m_state.update(m_state.current());
        return false;
    }
    m_lastUpdateTimeMs = t;

    KeypadState::store_t btnState = 0; // current state
    KeypadState::store_t btnStateMask = 1; // mask for current button
    uint16_t keyIndex = 0; // index of current button

    for (auto& row : m_rowPins)
    {
        row.read();
        row.write(0); // select row
        for (auto& col : m_colPins)
        {
            const auto pushed = col.read() == 0; // current button state
            const auto prev_pushed = (m_state.current() & btnStateMask) != 0; // get state on previous step
            auto& prev_time = m_updateTimes[keyIndex++];

            if ((pushed == prev_pushed) ||             // state not changed.
                (t - prev_time) > m_debouncingMs) {    // state changed, but long time ago, bouncing stopped
                if (pushed) { btnState |= btnStateMask; }
                prev_time = t;
            }

            btnStateMask <<= 1;
        }
        row.write(1); // deselect row
    }

    m_state.update(btnState);
    return m_state.changed();
}


/*
bool Keypad::update(Timestamp t)
{
    if(t - lastUpdateTime_ < debouncing_) {
        return false;
    }

    changed_ = lastUpdateTime_ == 0; // first time always changed
    lastUpdateTime_ = t;

    size_t keyIndex = 0;

    if (currRow >= rowPins_.size()) 
        currRow = 0;

    //keyIndex = currRow * colPins_.size();
    //auto& r = rowPins_[currRow++];

    btnState_ = 0;
    uint16_t btnStateMask = 1;

    for (auto& row : rowPins_)
    {
        //rowPins_[0] = 0; rowPins_[1] = 0; rowPins_[2] = 0; rowPins_[3] = 0;
        row.read();
        row.write(0); // select row
        //wait_us(10000);
        for (auto& col : colPins_)
        {
            auto& keyInfo = info_[keyIndex++];
            //col.mode(PinMode::PullDown);
            const auto pushed = col.read() == 0;
            //col.mode(PinMode::PullUp);

            if (pushed) {
                btnState_ |= btnStateMask;
                
                // workaround for strange problem: 
                // then you push any button in first row, 
                // first button also 'pushed'
                if (keyIndex > 1 && keyIndex < 4) {
                    //info_[0].state = IDLE;
                    //btnState_ &= ~1;
                }

                if (keyInfo.state == IDLE || keyInfo.state == RELEASED) {
                    keyInfo.state = PRESSED;
                    keyInfo.timestamp = t;
                    changed_ = true;
                }
                else if (keyInfo.state == PRESSED) {
                    keyInfo.state = HOLD;
                    keyInfo.timestamp = t;
                    changed_ = true;
                }
                //else if (keyInfo.state == PRESSED && (t - keyInfo.timestamp) > repeatInterval_) {
                //    keyInfo.state = REPEAT;
                //    keyInfo.timestamp = t;
                }
            }
            else {
                if (keyInfo.state == HOLD || keyInfo.state == PRESSED) {
                    keyInfo.state = RELEASED;
                    keyInfo.timestamp = t;
                    changed_ = true;
                } else if (keyInfo.state == RELEASED) {
                    keyInfo.state = IDLE;
                    keyInfo.timestamp = t;
                    changed_ = true;
                }
            }

            btnStateMask <<= 1;
        }
        row.write(1); // deselect row
    }

    return changed_;
}
*/

