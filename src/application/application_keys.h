// -*- coding:utf-8-unix -*-
/*!
    @file       application_keys.h
    @version    1.0
    @author     Demyan Davydov
    @contact    oi.zubnoy@gmail.com
    @date       2019-11-11

    @brief      Keys for keypad
    @description
        

    @see
    Copyright (C) 2019 Demyan Davydov.
    Released under the BSD 3-clause license.
*/

#pragma once
#ifndef file__APPLICATION_KEYS_H
#define file__APPLICATION_KEYS_H

enum VirtualKeys : char 
{
    VK_ENTER = '\n',
    VK_ESC   = 0x1b,
    VK_LEFT  = '~' + 1,
    VK_UP    = '~' + 2,
    VK_RIGHT = '~' + 3,
    VK_DOWN  = '~' + 4,
};

constexpr const char k_keypad_alphabet[] = 
{
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    VK_UP, VK_ENTER, VK_ESC, VK_LEFT, VK_DOWN, VK_RIGHT,
};


#endif // file__APPLICATION_KEYS_H
